import subprocess
import os

p = subprocess.Popen(['swig',
                      '-c++',
                      '-python',
                      'fastestrplidar.i'])
p.wait()

p = subprocess.Popen(['python3',
                      'setup.py',
                      'build_ext',
                      '--inplace'])
p.wait()

for file in os.listdir(os.getcwd()):
    if file.find('_fastestrplidar.cpython') >= 0:
        os.rename(os.path.join(os.getcwd(), file), os.path.join(os.getcwd(), '_fastestrplidar.so'))
        break