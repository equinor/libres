#!/usr/bin/env python
import os
import shutil
import os.path
import re

include_re = re.compile(r"#include\s+<ert/enkf/(\w+)\.h>")


def convert_cmake():
    pass


def rename_header(header_file):
    pass


def make_header(fname, root):
    with open(fname,"w") as f:
        f.write("#include <ert/enkf/{}.hpp>\n".format(root))


def update_includes(src_root):
    for path,dnames,fnames in os.walk(src_root):
        for f in fnames:
            _,ext = os.path.splitext(f)
            if ext != ".c":
                continue

            fname = os.path.join(path, f)
            lines = open(fname).readlines()
            print "Converting includes in:{}".format(fname)
            with open(fname,"w") as f:
                for line in lines:
                    match_obj = include_re.search(line)
                    if match_obj:
                        f.write("#include <ert/enkf/{}.hpp>\n".format(match_obj.group(1)))
                    else:
                        f.write(line)

        for d in dnames:
            update_includes(os.path.join(src_root, d))



def rename_headers(header_root):
    for path,dnames,fnames in os.walk(header_root):
        for f in fnames:
            root_name,ext = os.path.splitext(f)
            if ext != ".h":
                continue

            src_file = os.path.join(path, f)
            target_file = os.path.join(path , "{}.hpp".format(root_name))
            print "Moving {src} -> {target}".format(src = src_file,
                                                    target = target_file)
            shutil.move(src_file, target_file)
            make_header(src_file, root_name)



def rename_source(src_root):
    for path,dnames,fnames in os.walk(src_root):
        for f in fnames:
            root_name,ext = os.path.splitext(f)
            if ext != ".c":
                continue

            src_file = os.path.join(path, f)
            target_file = os.path.join(path , "{}.cpp".format(root_name))
            print "Moving {src} -> {target}".format(src = src_file,
                                                    target = target_file)
            shutil.move(src_file, target_file)

        for d in dnames:
            rename_source(os.path.join(src_root, d))




def main():
    header_root = "lib/include/ert/enkf"
    src_root = "lib/enkf"

    convert_cmake()
    rename_headers(header_root)
    update_includes(src_root)
    rename_source(src_root)


if __name__ == "__main__":
    main()
