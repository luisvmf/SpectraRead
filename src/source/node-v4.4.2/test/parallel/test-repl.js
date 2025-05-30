/* eslint-disable max-len, strict */
var common = require('../common');
var assert = require('assert');

common.globalCheck = false;
common.refreshTmpDir();

const net = require('net');
const repl = require('repl');
const message = 'Read, Eval, Print Loop';
const prompt_unix = 'node via Unix socket> ';
const prompt_tcp = 'node via TCP socket> ';
const prompt_multiline = '... ';
const prompt_npm = 'npm should be run outside of the ' +
                   'node repl, in your normal shell.\n' +
                   '(Press Control-D to exit.)\n';
const expect_npm = prompt_npm + prompt_unix;
var server_tcp, server_unix, client_tcp, client_unix, timer, replServer;


// absolute path to test/fixtures/a.js
var moduleFilename = require('path').join(common.fixturesDir, 'a');

console.error('repl test');

// function for REPL to run
invoke_me = function(arg) {
  return 'invoked ' + arg;
};

function send_expect(list) {
  if (list.length > 0) {
    var cur = list.shift();

    console.error('sending ' + JSON.stringify(cur.send));

    cur.client.expect = cur.expect;
    cur.client.list = list;
    if (cur.send.length > 0) {
      cur.client.write(cur.send + '\n');
    }
  }
}

function clean_up() {
  client_tcp.end();
  client_unix.end();
  clearTimeout(timer);
}

function strict_mode_error_test() {
  send_expect([
    { client: client_unix, send: 'ref = 1',
      expect: /^ReferenceError:\sref\sis\snot\sdefined\n\s+at\srepl:1:5/ },
  ]);
}

function error_test() {
  // The other stuff is done so reuse unix socket
  var read_buffer = '';
  var run_strict_test = true;
  client_unix.removeAllListeners('data');

  client_unix.on('data', function(data) {
    read_buffer += data.toString('ascii', 0, data.length);
    console.error('Unix data: ' + JSON.stringify(read_buffer) + ', expecting ' +
                 (client_unix.expect.exec ?
                  client_unix.expect :
                  JSON.stringify(client_unix.expect)));

    if (read_buffer.indexOf(prompt_unix) !== -1) {
      // if it's an exact match, then don't do the regexp
      if (read_buffer !== client_unix.expect) {
        var expect = client_unix.expect;
        if (expect === prompt_multiline)
          expect = /[\.]{3} /;
        assert.ok(read_buffer.match(expect));
        console.error('match');
      }
      read_buffer = '';
      if (client_unix.list && client_unix.list.length > 0) {
        send_expect(client_unix.list);
      } else if (run_strict_test) {
        replServer.replMode = repl.REPL_MODE_STRICT;
        run_strict_test = false;
        strict_mode_error_test();
      } else {
        console.error('End of Error test, running TCP test.');
        tcp_test();
      }

    } else if (read_buffer.indexOf(prompt_multiline) !== -1) {
      // Check that you meant to send a multiline test
      assert.strictEqual(prompt_multiline, client_unix.expect);
      read_buffer = '';
      if (client_unix.list && client_unix.list.length > 0) {
        send_expect(client_unix.list);
      } else if (run_strict_test) {
        replServer.replMode = repl.REPL_MODE_STRICT;
        run_strict_test = false;
        strict_mode_error_test();
      } else {
        console.error('End of Error test, running TCP test.\n');
        tcp_test();
      }

    } else {
      console.error('didn\'t see prompt yet, buffering.');
    }
  });

  send_expect([
    // Uncaught error throws and prints out
    { client: client_unix, send: 'throw new Error(\'test error\');',
      expect: /^Error: test error/ },
    // Common syntax error is treated as multiline command
    { client: client_unix, send: 'function test_func() {',
      expect: prompt_multiline },
    // You can recover with the .break command
    { client: client_unix, send: '.break',
      expect: prompt_unix },
    // But passing the same string to eval() should throw
    { client: client_unix, send: 'eval("function test_func() {")',
      expect: /^SyntaxError: Unexpected end of input/ },
    // Can handle multiline template literals
    { client: client_unix, send: '`io.js',
      expect: prompt_multiline },
    // Special REPL commands still available
    { client: client_unix, send: '.break',
      expect: prompt_unix },
    // Template expressions can cross lines
    { client: client_unix, send: '`io.js ${"1.0"',
      expect: prompt_multiline },
    { client: client_unix, send: '+ ".2"}`',
      expect: `'io.js 1.0.2'\n${prompt_unix}` },
    // Floating point numbers are not interpreted as REPL commands.
    { client: client_unix, send: '.1234',
      expect: '0.1234' },
    // Floating point expressions are not interpreted as REPL commands
    { client: client_unix, send: '.1+.1',
      expect: '0.2' },
    // Can parse valid JSON
    { client: client_unix, send: 'JSON.parse(\'{"valid": "json"}\');',
      expect: '{ valid: \'json\' }'},
    // invalid input to JSON.parse error is special case of syntax error,
    // should throw
    { client: client_unix, send: 'JSON.parse(\'{invalid: \\\'json\\\'}\');',
      expect: /^SyntaxError: Unexpected token i/ },
    // end of input to JSON.parse error is special case of syntax error,
    // should throw
    { client: client_unix, send: 'JSON.parse(\'066\');',
      expect: /^SyntaxError: Unexpected number/ },
    // should throw
    { client: client_unix, send: 'JSON.parse(\'{\');',
      expect: /^SyntaxError: Unexpected end of input/ },
    // invalid RegExps are a special case of syntax error,
    // should throw
    { client: client_unix, send: '/(/;',
      expect: /^SyntaxError: Invalid regular expression\:/ },
    // invalid RegExp modifiers are a special case of syntax error,
    // should throw (GH-4012)
    { client: client_unix, send: 'new RegExp("foo", "wrong modifier");',
      expect: /^SyntaxError: Invalid flags supplied to RegExp constructor/ },
    // strict mode syntax errors should be caught (GH-5178)
    { client: client_unix, send: '(function() { "use strict"; return 0755; })()',
      expect: /^SyntaxError: Octal literals are not allowed in strict mode/ },
    { client: client_unix, send: '(function(a, a, b) { "use strict"; return a + b + c; })()',
      expect: /^SyntaxError: Strict mode function may not have duplicate parameter names/ },
    { client: client_unix, send: '(function() { "use strict"; with (this) {} })()',
      expect: /^SyntaxError: Strict mode code may not include a with statement/ },
    { client: client_unix, send: '(function() { "use strict"; var x; delete x; })()',
      expect: /^SyntaxError: Delete of an unqualified identifier in strict mode/ },
    { client: client_unix, send: '(function() { "use strict"; eval = 17; })()',
      expect: /^SyntaxError: Unexpected eval or arguments in strict mode/ },
    { client: client_unix, send: '(function() { "use strict"; if (true) function f() { } })()',
      expect: /^SyntaxError: In strict mode code, functions can only be declared at top level or immediately within another function/ },
    // Named functions can be used:
    { client: client_unix, send: 'function blah() { return 1; }',
      expect: prompt_unix },
    { client: client_unix, send: 'blah()',
      expect: '1\n' + prompt_unix },
    // Functions should not evaluate twice (#2773)
    { client: client_unix, send: 'var I = [1,2,3,function() {}]; I.pop()',
      expect: '[Function]' },
    // Multiline object
    { client: client_unix, send: '{ a: ',
      expect: prompt_multiline },
    { client: client_unix, send: '1 }',
      expect: '{ a: 1 }' },
    // Multiline anonymous function with comment
    { client: client_unix, send: '(function() {',
      expect: prompt_multiline },
    { client: client_unix, send: '// blah',
      expect: prompt_multiline },
    { client: client_unix, send: 'return 1;',
      expect: prompt_multiline },
    { client: client_unix, send: '})()',
      expect: '1' },
    // npm prompt error message
    { client: client_unix, send: 'npm install foobar',
      expect: expect_npm },
    { client: client_unix, send: '(function() {\n\nreturn 1;\n})()',
      expect: '1' },
    { client: client_unix, send: '{\n\na: 1\n}',
      expect: '{ a: 1 }' },
    { client: client_unix, send: 'url.format("http://google.com")',
      expect: 'http://google.com/' },
    { client: client_unix, send: 'var path = 42; path',
      expect: '42' },
    // this makes sure that we don't print `undefined` when we actually print
    // the error message
    { client: client_unix, send: '.invalid_repl_command',
      expect: 'Invalid REPL keyword\n' + prompt_unix },
    // this makes sure that we don't crash when we use an inherited property as
    // a REPL command
    { client: client_unix, send: '.toString',
      expect: 'Invalid REPL keyword\n' + prompt_unix },
    // fail when we are not inside a String and a line continuation is used
    { client: client_unix, send: '[] \\',
      expect: /^SyntaxError: Unexpected token ILLEGAL/ },
    // do not fail when a String is created with line continuation
    { client: client_unix, send: '\'the\\\nfourth\\\neye\'',
      expect: prompt_multiline + prompt_multiline +
              '\'thefourtheye\'\n' + prompt_unix },
    // Don't fail when a partial String is created and line continuation is used
    // with whitespace characters at the end of the string. We are to ignore it.
    // This test is to make sure that we properly remove the whitespace
    // characters at the end of line, unlike the buggy `trimWhitespace` function
    { client: client_unix, send: '  \t    .break  \t  ',
      expect: prompt_unix },
    // multiline strings preserve whitespace characters in them
    { client: client_unix, send: '\'the \\\n   fourth\t\t\\\n  eye  \'',
      expect: prompt_multiline + prompt_multiline +
              '\'the    fourth\\t\\t  eye  \'\n' + prompt_unix },
    // more than one multiline strings also should preserve whitespace chars
    { client: client_unix, send: '\'the \\\n   fourth\' +  \'\t\t\\\n  eye  \'',
      expect: prompt_multiline + prompt_multiline +
              '\'the    fourth\\t\\t  eye  \'\n' + prompt_unix },
    // using REPL commands within a string literal should still work
    { client: client_unix, send: '\'\\\n.break',
      expect: prompt_unix },
    // using REPL command "help" within a string literal should still work
    { client: client_unix, send: '\'thefourth\\\n.help\neye\'',
      expect: /'thefourtheye'/ },
    // empty lines in the REPL should be allowed
    { client: client_unix, send: '\n\r\n\r\n',
      expect: prompt_unix + prompt_unix + prompt_unix },
    // empty lines in the string literals should not affect the string
    { client: client_unix, send: '\'the\\\n\\\nfourtheye\'\n',
      expect: prompt_multiline + prompt_multiline +
              '\'thefourtheye\'\n' + prompt_unix },
    // Regression test for https://github.com/nodejs/node/issues/597
    { client: client_unix,
      send: '/(.)(.)(.)(.)(.)(.)(.)(.)(.)/.test(\'123456789\')\n',
      expect: `true\n${prompt_unix}` },
    // the following test's result depends on the RegEx's match from the above
    { client: client_unix,
      send: 'RegExp.$1\nRegExp.$2\nRegExp.$3\nRegExp.$4\nRegExp.$5\n' +
            'RegExp.$6\nRegExp.$7\nRegExp.$8\nRegExp.$9\n',
      expect: ['\'1\'\n', '\'2\'\n', '\'3\'\n', '\'4\'\n', '\'5\'\n', '\'6\'\n',
               '\'7\'\n', '\'8\'\n', '\'9\'\n'].join(`${prompt_unix}`) },
    // regression tests for https://github.com/nodejs/node/issues/2749
    { client: client_unix, send: 'function x() {\nreturn \'\\n\';\n }',
      expect: prompt_multiline + prompt_multiline +
              'undefined\n' + prompt_unix },
    { client: client_unix, send: 'function x() {\nreturn \'\\\\\';\n }',
      expect: prompt_multiline + prompt_multiline +
              'undefined\n' + prompt_unix },
    // regression tests for https://github.com/nodejs/node/issues/3421
    { client: client_unix, send: 'function x() {\n//\'\n }',
      expect: prompt_multiline + prompt_multiline +
              'undefined\n' + prompt_unix },
    { client: client_unix, send: 'function x() {\n//"\n }',
      expect: prompt_multiline + prompt_multiline +
              'undefined\n' + prompt_unix },
    { client: client_unix, send: 'function x() {//\'\n }',
      expect: prompt_multiline + 'undefined\n' + prompt_unix },
    { client: client_unix, send: 'function x() {//"\n }',
      expect: prompt_multiline + 'undefined\n' + prompt_unix },
    { client: client_unix, send: 'function x() {\nvar i = "\'";\n }',
      expect: prompt_multiline + prompt_multiline +
              'undefined\n' + prompt_unix },
    { client: client_unix, send: 'function x(/*optional*/) {}',
      expect: 'undefined\n' + prompt_unix },
    { client: client_unix, send: 'function x(/* // 5 */) {}',
      expect: 'undefined\n' + prompt_unix },
    { client: client_unix, send: '// /* 5 */',
      expect: 'undefined\n' + prompt_unix },
    { client: client_unix, send: '"//"',
      expect: '\'//\'\n' + prompt_unix },
    { client: client_unix, send: '"data /*with*/ comment"',
      expect: '\'data /*with*/ comment\'\n' + prompt_unix },
    { client: client_unix, send: 'function x(/*fn\'s optional params*/) {}',
      expect: 'undefined\n' + prompt_unix },
    { client: client_unix, send: '/* \'\n"\n\'"\'\n*/',
      expect: 'undefined\n' + prompt_unix },
    // REPL should get a normal require() function, not one that allows
    // access to internal modules without the --expose_internals flag.
    { client: client_unix, send: 'require("internal/repl")',
      expect: /^Error: Cannot find module 'internal\/repl'/ },
    // REPL should handle quotes within regexp literal in multiline mode
    { client: client_unix, send: "function x(s) {\nreturn s.replace(/'/,'');\n}",
      expect: prompt_multiline + prompt_multiline +
            'undefined\n' + prompt_unix },
    { client: client_unix, send: "function x(s) {\nreturn s.replace(/\'/,'');\n}",
      expect: prompt_multiline + prompt_multiline +
            'undefined\n' + prompt_unix },
    { client: client_unix, send: 'function x(s) {\nreturn s.replace(/"/,"");\n}',
      expect: prompt_multiline + prompt_multiline +
            'undefined\n' + prompt_unix },
    { client: client_unix, send: 'function x(s) {\nreturn s.replace(/.*/,"");\n}',
      expect: prompt_multiline + prompt_multiline +
            'undefined\n' + prompt_unix },
  ]);
}

function tcp_test() {
  server_tcp = net.createServer(function(socket) {
    assert.strictEqual(server_tcp, socket.server);

    socket.on('end', function() {
      socket.end();
    });

    repl.start(prompt_tcp, socket);
  });

  server_tcp.listen(common.PORT, function() {
    var read_buffer = '';

    client_tcp = net.createConnection(common.PORT);

    client_tcp.on('connect', function() {
      assert.equal(true, client_tcp.readable);
      assert.equal(true, client_tcp.writable);

      send_expect([
        { client: client_tcp, send: '',
          expect: prompt_tcp },
        { client: client_tcp, send: 'invoke_me(333)',
          expect: ('\'' + 'invoked 333' + '\'\n' + prompt_tcp) },
        { client: client_tcp, send: 'a += 1',
          expect: ('12346' + '\n' + prompt_tcp) },
        { client: client_tcp,
          send: 'require(' + JSON.stringify(moduleFilename) + ').number',
          expect: ('42' + '\n' + prompt_tcp) }
      ]);
    });

    client_tcp.on('data', function(data) {
      read_buffer += data.toString('ascii', 0, data.length);
      console.error('TCP data: ' + JSON.stringify(read_buffer) +
                   ', expecting ' + JSON.stringify(client_tcp.expect));
      if (read_buffer.indexOf(prompt_tcp) !== -1) {
        assert.strictEqual(client_tcp.expect, read_buffer);
        console.error('match');
        read_buffer = '';
        if (client_tcp.list && client_tcp.list.length > 0) {
          send_expect(client_tcp.list);
        } else {
          console.error('End of TCP test.\n');
          clean_up();
        }
      } else {
        console.error('didn\'t see prompt yet, buffering');
      }
    });

    client_tcp.on('error', function(e) {
      throw e;
    });

    client_tcp.on('close', function() {
      server_tcp.close();
    });
  });

}

function unix_test() {
  server_unix = net.createServer(function(socket) {
    assert.strictEqual(server_unix, socket.server);

    socket.on('end', function() {
      socket.end();
    });

    replServer = repl.start({
      prompt: prompt_unix,
      input: socket,
      output: socket,
      useGlobal: true
    });
    replServer.context.message = message;
  });

  server_unix.on('listening', function() {
    var read_buffer = '';

    client_unix = net.createConnection(common.PIPE);

    client_unix.on('connect', function() {
      assert.equal(true, client_unix.readable);
      assert.equal(true, client_unix.writable);

      send_expect([
        { client: client_unix, send: '',
          expect: prompt_unix },
        { client: client_unix, send: 'message',
          expect: ('\'' + message + '\'\n' + prompt_unix) },
        { client: client_unix, send: 'invoke_me(987)',
          expect: ('\'' + 'invoked 987' + '\'\n' + prompt_unix) },
        { client: client_unix, send: 'a = 12345',
          expect: ('12345' + '\n' + prompt_unix) },
        { client: client_unix, send: '{a:1}',
          expect: ('{ a: 1 }' + '\n' + prompt_unix) }
      ]);
    });

    client_unix.on('data', function(data) {
      read_buffer += data.toString('ascii', 0, data.length);
      console.error('Unix data: ' + JSON.stringify(read_buffer) +
                   ', expecting ' + JSON.stringify(client_unix.expect));
      if (read_buffer.indexOf(prompt_unix) !== -1) {
        assert.strictEqual(client_unix.expect, read_buffer);
        console.error('match');
        read_buffer = '';
        if (client_unix.list && client_unix.list.length > 0) {
          send_expect(client_unix.list);
        } else {
          console.error('End of Unix test, running Error test.\n');
          process.nextTick(error_test);
        }
      } else {
        console.error('didn\'t see prompt yet, buffering.');
      }
    });

    client_unix.on('error', function(e) {
      throw e;
    });

    client_unix.on('close', function() {
      server_unix.close();
    });
  });

  server_unix.listen(common.PIPE);
}

unix_test();

timer = setTimeout(function() {
  assert.fail(null, null, 'Timeout');
}, 5000);
