#!/usr/bin/env python3
# Simple tests for the talloc python bindings.
# Copyright (C) 2015 Petr Viktorin <pviktori@redhat.com>

import unittest
import subprocess
import sys
import gc

import talloc
import _test_pytalloc


def dummy_func():
    pass


class TallocTests(unittest.TestCase):

    def test_report_full(self):
        # report_full is hardcoded to print to stdout, so use a subprocess
        process = subprocess.Popen([
            sys.executable, '-c',
            """if True:
            import talloc, _test_pytalloc
            obj = _test_pytalloc.new()
            talloc.report_full(obj)
            """
        ], stdout=subprocess.PIPE)
        output, stderr = process.communicate()
        output = str(output)
        self.assertTrue("full talloc report on 'talloc.Object" in output)
        self.assertTrue("This is a test string" in output)

    def test_totalblocks(self):
        obj = _test_pytalloc.new()
        # Two blocks: the string, and the name
        self.assertEqual(talloc.total_blocks(obj), 2)

    def test_repr(self):
        obj = _test_pytalloc.new()
        prefix = '<talloc.Object talloc object at'
        self.assertTrue(repr(obj).startswith(prefix))
        self.assertEqual(repr(obj), str(obj))

    def test_base_repr(self):
        obj = _test_pytalloc.base_new()
        prefix = '<talloc.BaseObject talloc based object at'
        self.assertTrue(repr(obj).startswith(prefix))
        self.assertEqual(repr(obj), str(obj))

    def test_destructor(self):
        # Check correct lifetime of the talloc'd data
        lst = []
        obj = _test_pytalloc.DObject(lambda: lst.append('dead'))
        self.assertEqual(lst, [])
        del obj
        gc.collect()
        self.assertEqual(lst, ['dead'])

    def test_base_destructor(self):
        # Check correct lifetime of the talloc'd data
        lst = []
        obj = _test_pytalloc.DBaseObject(lambda: lst.append('dead'))
        self.assertEqual(lst, [])
        del obj
        gc.collect()
        self.assertEqual(lst, ['dead'])


class TallocComparisonTests(unittest.TestCase):

    def test_compare_same(self):
        obj1 = _test_pytalloc.new()
        self.assertTrue(obj1 == obj1)
        self.assertFalse(obj1 != obj1)
        self.assertTrue(obj1 <= obj1)
        self.assertFalse(obj1 < obj1)
        self.assertTrue(obj1 >= obj1)
        self.assertFalse(obj1 > obj1)

    def test_compare_different(self):
        # object comparison is consistent
        obj1, obj2 = sorted([
            _test_pytalloc.new(),
            _test_pytalloc.new()])
        self.assertFalse(obj1 == obj2)
        self.assertTrue(obj1 != obj2)
        self.assertTrue(obj1 <= obj2)
        self.assertTrue(obj1 < obj2)
        self.assertFalse(obj1 >= obj2)
        self.assertFalse(obj1 > obj2)


class TallocBaseComparisonTests(unittest.TestCase):

    def test_compare_same(self):
        obj1 = _test_pytalloc.base_new()
        self.assertTrue(obj1 == obj1)
        self.assertFalse(obj1 != obj1)
        self.assertTrue(obj1 <= obj1)
        self.assertFalse(obj1 < obj1)
        self.assertTrue(obj1 >= obj1)
        self.assertFalse(obj1 > obj1)

    def test_compare_different(self):
        # object comparison is consistent
        obj1, obj2 = sorted([
            _test_pytalloc.base_new(),
            _test_pytalloc.base_new()])
        self.assertFalse(obj1 == obj2)
        self.assertTrue(obj1 != obj2)
        self.assertTrue(obj1 <= obj2)
        self.assertTrue(obj1 < obj2)
        self.assertFalse(obj1 >= obj2)
        self.assertFalse(obj1 > obj2)


class TallocUtilTests(unittest.TestCase):

    def test_get_type(self):
        self.assertTrue(talloc.Object is _test_pytalloc.get_object_type())

    def test_reference(self):
        # Check correct lifetime of the talloc'd data with multiple references
        lst = []
        obj = _test_pytalloc.DObject(lambda: lst.append('dead'))
        ref = _test_pytalloc.reference(obj)
        del obj
        gc.collect()
        self.assertEqual(lst, [])
        del ref
        gc.collect()
        self.assertEqual(lst, ['dead'])

    def test_get_base_type(self):
        self.assertTrue(talloc.BaseObject is _test_pytalloc.base_get_object_type())

    def test_base_reference(self):
        # Check correct lifetime of the talloc'd data with multiple references
        lst = []
        obj = _test_pytalloc.DBaseObject(lambda: lst.append('dead'))
        ref = _test_pytalloc.base_reference(obj)
        del obj
        gc.collect()
        self.assertEqual(lst, [])
        del ref
        gc.collect()
        self.assertEqual(lst, ['dead'])


if __name__ == '__main__':
    unittest.TestProgram()
