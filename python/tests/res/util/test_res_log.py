import os
from res.util import ResLog, Log
from tests import ResTest, tmpdir
from res.util.enums import MessageLevelEnum


class ResLogTest(ResTest):
    @tmpdir()
    def test_log(self):
        test_log_filename = "test_log"
        ResLog.init(1, test_log_filename, True)
        message = "This is fun"
        ResLog.log(1, message)

        self.assertTrue(os.path.isfile(test_log_filename))

        with open(test_log_filename, "r") as f:
            text = f.readlines()
            self.assertTrue(len(text) > 0)
            self.assertTrue(message in text[-1])

    @tmpdir()
    def test_getFilename(self):
        test_log_filename = "log_test_file.txt"
        ResLog.init(1, test_log_filename, True)
        message = "This is fun"
        ResLog.log(1, message)

        self.assertEqual(ResLog.getFilename(), test_log_filename)

    @tmpdir()
    def test_log2(self):
        logh = Log("logfile", MessageLevelEnum.LOG_DEBUG)

        os.mkdir("read_only")
        os.chmod("read_only", 0o500)
        with self.assertRaises(IOError):
            logh = Log("read_only/logfile.txt", MessageLevelEnum.LOG_DEBUG)

    @tmpdir()
    def test_init_perm_denied(self):
        os.mkdir("read_only")
        os.chmod("read_only", 0o500)
        with self.assertRaises(IOError):
            ResLog.init(1, "read_only/logfile.txt", True)
