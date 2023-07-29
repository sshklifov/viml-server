#!/usr/bin/python3

import subprocess
import re

def get_cmds_output(*cmds, err=False, stdin_str=""):
    subprocess_args = {'args': cmds, 'stdout': subprocess.PIPE, 'stderr': subprocess.PIPE}
    if len(stdin_str) > 0:
        subprocess_args['input'] = bytes(stdin_str, 'utf-8')
    if len(cmds) <= 1:
        subprocess_args['shell'] = True
    completed_process = subprocess.run(**subprocess_args)

    stdout = completed_process.stdout.decode('utf-8')
    stderr = completed_process.stderr.decode('utf-8')
    if completed_process.returncode == 0:
        return stderr if err else stdout
    else:
        raise Exception("Error executing Nvim command:\n" + stderr)

def get_nvim_cmds_output(*cmds):
    nvim_cmd = ["nvim", "--clean", "--headless"]
    for cmd in cmds:
        nvim_cmd.extend(["-c", cmd])
    nvim_cmd.extend(["-c", "qall!"])

    return get_cmds_output(*nvim_cmd, err=True)

def get_all_cmds():
    index_txt = get_nvim_cmds_output("help index.txt", "echo expand('%:p')")

    fp = open(index_txt)
    index_txt_lines = fp.read()
    fp.close()

    cmds = re.findall("[|]([:][a-z]+)[|]", index_txt_lines)
    return set(cmds)

def get_all_help_files():
    cmds = get_all_cmds()
    help_files = set()
    for cmd in cmds:
        file = get_nvim_cmds_output(f"help {cmd}", "echo expand('%:p')")
        help_files.add(file)
    return help_files

def get_all_cmd_from_help():
    cmds_found = get_cmds_output(f"./help < ex_cmds.lua").strip().split("\n")
    # raise Exception(cmds_found)
    return cmds_found

good_cmd = get_all_cmd_from_help()
all_cmd = get_all_cmds()
remaining_cmd = all_cmd.difference(good_cmd)

for cmd in remaining_cmd:
    print(cmd)
# index_txt = get_nvim_cmds_output("help index.txt", "echo expand('%:p')")
# print(index_txt)
# for file in get_all_help_files():
#     print(file)
#print(get_all_cmds())

# def get_all_quickref_cmds():
#     quickref_txt = get_nvim_cmds_output("help quickref.txt", "echo expand('%:p')")

#     fp = open(quickref_txt)
#     quickref_txt_lines = fp.read()
#     fp.close()

#     cmds = re.findall("([|][:][a-z]+[|].*)\n", quickref_txt_lines)
#     return cmds
    # cmds = list(set(cmds))
    # return cmds

# def get_proto():
#     help_txt = get_nvim_cmds_output("help :mksession", "echo expand('%:p')")
#     fp = open(help_txt)
    # help_lines = fp.read()
    # fp.close()

    # cmds = re.findall("([|][:][a-z[]!]+[|].*)\n", quickref_txt_lines)
    # return cmds
    # cmds = list(set(cmds))
    # return cmds



import sys
sys.exit(0)
# lines = get_cmds_output('grep', '^|:[^\\s]*|', index_txt).strip().split("\n")



dup_cmds = get_cmds_output("cut -d '|' -f 2 | sort -u", stdin_str=cmd_entries).strip().split("\n")
print(res)


# stdout = completed_process.stdout.decode('utf-8')
# stderr = completed_process.stderr.decode('utf-8')
# if completed_process.returncode == 0:
#     print(stdout)
# else:
#     raise Exception("Error executing Nvim command:\n" + stderr)

# print(cmds)
