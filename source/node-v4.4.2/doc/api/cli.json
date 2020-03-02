{
  "source": "doc/api/cli.markdown",
  "miscs": [
    {
      "textRaw": "Command Line Options",
      "name": "Command Line Options",
      "type": "misc",
      "desc": "<p>Node.js comes with a wide variety of CLI options. These options expose built-in\ndebugging, multiple ways to execute scripts, and other helpful runtime options.\n\n</p>\n<p>To view this documentation as a manual page in your terminal, run <code>man node</code>.\n\n\n</p>\n",
      "miscs": [
        {
          "textRaw": "Synopsis",
          "name": "synopsis",
          "desc": "<p><code>node [options] [v8 options] [script.js | -e &quot;script&quot;] [arguments]</code>\n\n</p>\n<p><code>node debug [script.js | -e &quot;script&quot; | &lt;host&gt;:&lt;port&gt;] …</code>\n\n</p>\n<p><code>node --v8-options</code>\n\n</p>\n<p>Execute without arguments to start the [REPL][].\n\n</p>\n<p><em>For more info about <code>node debug</code>, please see the [debugger][] documentation.</em>\n\n\n</p>\n",
          "type": "misc",
          "displayName": "Synopsis"
        },
        {
          "textRaw": "Options",
          "name": "options",
          "modules": [
            {
              "textRaw": "`-v`, `--version`",
              "name": "`-v`,_`--version`",
              "desc": "<p>Print node&#39;s version.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`-v`, `--version`"
            },
            {
              "textRaw": "`-h`, `--help`",
              "name": "`-h`,_`--help`",
              "desc": "<p>Print node command line options.\nThe output of this option is less detailed than this document.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`-h`, `--help`"
            },
            {
              "textRaw": "`-e`, `--eval \"script\"`",
              "name": "`-e`,_`--eval_\"script\"`",
              "desc": "<p>Evaluate the following argument as JavaScript.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`-e`, `--eval \"script\"`"
            },
            {
              "textRaw": "`-p`, `--print \"script\"`",
              "name": "`-p`,_`--print_\"script\"`",
              "desc": "<p>Identical to <code>-e</code> but prints the result.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`-p`, `--print \"script\"`"
            },
            {
              "textRaw": "`-c`, `--check`",
              "name": "`-c`,_`--check`",
              "desc": "<p>Syntax check the script without executing.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`-c`, `--check`"
            },
            {
              "textRaw": "`-i`, `--interactive`",
              "name": "`-i`,_`--interactive`",
              "desc": "<p>Opens the REPL even if stdin does not appear to be a terminal.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`-i`, `--interactive`"
            },
            {
              "textRaw": "`-r`, `--require module`",
              "name": "`-r`,_`--require_module`",
              "desc": "<p>Preload the specified module at startup.\n\n</p>\n<p>Follows <code>require()</code>&#39;s module resolution\nrules. <code>module</code> may be either a path to a file, or a node module name.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`-r`, `--require module`"
            },
            {
              "textRaw": "`--no-deprecation`",
              "name": "`--no-deprecation`",
              "desc": "<p>Silence deprecation warnings.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--no-deprecation`"
            },
            {
              "textRaw": "`--trace-deprecation`",
              "name": "`--trace-deprecation`",
              "desc": "<p>Print stack traces for deprecations.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--trace-deprecation`"
            },
            {
              "textRaw": "`--throw-deprecation`",
              "name": "`--throw-deprecation`",
              "desc": "<p>Throw errors for deprecations.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--throw-deprecation`"
            },
            {
              "textRaw": "`--trace-sync-io`",
              "name": "`--trace-sync-io`",
              "desc": "<p>Prints a stack trace whenever synchronous I/O is detected after the first turn\nof the event loop.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--trace-sync-io`"
            },
            {
              "textRaw": "`--zero-fill-buffers`",
              "name": "`--zero-fill-buffers`",
              "desc": "<p>Automatically zero-fills all newly allocated [Buffer][] and [SlowBuffer][]\ninstances.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--zero-fill-buffers`"
            },
            {
              "textRaw": "`--track-heap-objects`",
              "name": "`--track-heap-objects`",
              "desc": "<p>Track heap object allocations for heap snapshots.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--track-heap-objects`"
            },
            {
              "textRaw": "`--prof-process`",
              "name": "`--prof-process`",
              "desc": "<p>Process v8 profiler output generated using the v8 option <code>--prof</code>.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--prof-process`"
            },
            {
              "textRaw": "`--v8-options`",
              "name": "`--v8-options`",
              "desc": "<p>Print v8 command line options.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--v8-options`"
            },
            {
              "textRaw": "`--tls-cipher-list=list`",
              "name": "`--tls-cipher-list=list`",
              "desc": "<p>Specify an alternative default TLS cipher list. (Requires Node.js to be built\nwith crypto support. (Default))\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--tls-cipher-list=list`"
            },
            {
              "textRaw": "`--enable-fips`",
              "name": "`--enable-fips`",
              "desc": "<p>Enable FIPS-compliant crypto at startup. (Requires Node.js to be built with\n<code>./configure --openssl-fips</code>)\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--enable-fips`"
            },
            {
              "textRaw": "`--force-fips`",
              "name": "`--force-fips`",
              "desc": "<p>Force FIPS-compliant crypto on startup. (Cannot be disabled from script code.)\n(Same requirements as <code>--enable-fips</code>)\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--force-fips`"
            },
            {
              "textRaw": "`--icu-data-dir=file`",
              "name": "`--icu-data-dir=file`",
              "desc": "<p>Specify ICU data load path. (overrides <code>NODE_ICU_DATA</code>)\n\n\n</p>\n",
              "type": "module",
              "displayName": "`--icu-data-dir=file`"
            }
          ],
          "type": "misc",
          "displayName": "Options"
        },
        {
          "textRaw": "Environment Variables",
          "name": "environment_variables",
          "modules": [
            {
              "textRaw": "`NODE_DEBUG=module[,…]`",
              "name": "`node_debug=module[,…]`",
              "desc": "<p><code>&#39;,&#39;</code>-separated list of core modules that should print debug information.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`NODE_DEBUG=module[,…]`"
            },
            {
              "textRaw": "`NODE_PATH=path[:…]`",
              "name": "`node_path=path[:…]`",
              "desc": "<p><code>&#39;:&#39;</code>-separated list of directories prefixed to the module search path.\n\n</p>\n<p><em>Note: on Windows, this is a <code>&#39;;&#39;</code>-separated list instead.</em>\n\n\n</p>\n",
              "type": "module",
              "displayName": "`NODE_PATH=path[:…]`"
            },
            {
              "textRaw": "`NODE_DISABLE_COLORS=1`",
              "name": "`node_disable_colors=1`",
              "desc": "<p>When set to <code>1</code> colors will not be used in the REPL.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`NODE_DISABLE_COLORS=1`"
            },
            {
              "textRaw": "`NODE_ICU_DATA=file`",
              "name": "`node_icu_data=file`",
              "desc": "<p>Data path for ICU (Intl object) data. Will extend linked-in data when compiled\nwith small-icu support.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`NODE_ICU_DATA=file`"
            },
            {
              "textRaw": "`NODE_REPL_HISTORY=file`",
              "name": "`node_repl_history=file`",
              "desc": "<p>Path to the file used to store the persistent REPL history. The default path is\n<code>~/.node_repl_history</code>, which is overridden by this variable. Setting the value\nto an empty string (<code>&quot;&quot;</code> or <code>&quot; &quot;</code>) disables persistent REPL history.\n\n\n</p>\n",
              "type": "module",
              "displayName": "`NODE_REPL_HISTORY=file`"
            }
          ],
          "type": "misc",
          "displayName": "Environment Variables"
        }
      ]
    }
  ]
}
