#!/usr/bin/env python
import os
import shutil
import os.path
import re

include_re = re.compile(r"#include\s+<ert/enkf/(\w+)\.h>")

cmake_src_re = re.compile(r"enkf/(\w+)\.c")
return_re = re.compile(r"(?P<space>\s*)(?P<const>const )?(?P<type>\w+)\s*\*\s*(?P<var>\w+)\s*=\s*(?P<func>.+)\s*;\s*$")


def transform_return(line):
    func_list = ["vector_iget",
                 "vector_iget_const",
                 "hash_get",
                 "hash_get_const",
                 "int_vector_alloc_copy",
                 "util_malloc",
                 "util_alloc_copy",
                 "enkf_config_node_get_ref",
                 "vector_get_last",
                 "queue_driver_get_option",
                 "util_calloc",
                 "vector_safe_iget"]

    white_list = ["hash_get_size", "hash_get_double", "hash_get_int", "hash_get_counter"]

    for func in white_list:
        if line.find(func) >= 0:
            return line

    for func in func_list:
        if line.find(" {}".format(func)) >= 0:
            match_obj = return_re.search(line)
            if match_obj:
                space = match_obj.group("space")
                const = match_obj.group("const")
                ctype = match_obj.group("type")
                var = match_obj.group("var")
                func = match_obj.group("func")

                if const is None:
                    const = ""

                line = "{space}{const}{ctype} * {var} = ({const}{ctype} *){func};\n".format( space = space,
                                                                                             const = const,
                                                                                             ctype = ctype,
                                                                                             var = var,
                                                                                             func = func)
                return line
            else:
                print "match error: %s" % line
                return "{} // CXX_CAST_ERROR\n".format(line[:-1])
    return line



def convert_cmake(cmake_file):
    lines = open(cmake_file).readlines()
    with open(cmake_file, "w") as f:
        for line in lines:
            match_obj = cmake_src_re.search(line)
            if match_obj:
                f.write("enkf/{}.cpp\n".format(match_obj.group(1)))
            else:
                f.write(line)


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
            with open(target_file, "w") as of:
                with open(src_file) as inf:
                    for line in inf.readlines():
                        line = transform_return(line)

                        of.write(line)

        for d in dnames:
            rename_source(os.path.join(src_root, d))




def main():
    header_root = "lib/include/ert/enkf"
    src_root = "lib/enkf"

    rename_headers(header_root)
    update_includes(src_root)
    rename_source(src_root)
    convert_cmake("lib/CMakeLists.txt")

if __name__ == "__main__":
    main()
