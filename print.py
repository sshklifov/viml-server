#!/usr/bin/python3

import subprocess
import os

directory = os.path.expanduser("~/Downloads/awesome")
files = subprocess.check_output(f"find {directory} -type f -regex '.*[.]vim'", shell=True, text=True).strip().split("\n")
for file in files:
    # print(file)
    exe = os.path.abspath("./Debug/lsp/lsp")
    subprocess.run(f"{exe} {file}", shell=True, text=True)
