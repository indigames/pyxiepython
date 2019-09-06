try:
    importlib.import_module('numpy')
except ImportError:
	from pip._internal import main as _main
	_main(['install', 'numpy'])

from setuptools import setup, Extension, find_packages
import setuptools
import numpy
import sys
from distutils.sysconfig import get_python_lib


is64Bit = sys.maxsize > 2 ** 32

if is64Bit:
    bindir = 'bin/win64'
else:
    bindir = 'bin/win32'

pyxie_module = Extension('pyxie._pyxie', 
                       sources=[
                           'Backyard.cpp',
                           'pythonEnvironment.cpp',
                           'pyxieFile.cpp',
                           'pythonEditableFigure.cpp',
                           'Window.cpp',
                           'pythonShowcase.cpp',
                           'pythonAnimator.cpp',
                           'pythonModule.cpp',
                           'pythonCamera.cpp',
                           'pythonShaderGenerator.cpp',
                           'pythonFigure.cpp',
                           'pythonResource.cpp',
                           'pythonTexture.cpp'
                       ],
                       include_dirs=['bin/include', numpy.get_include()],
			           library_dirs=[bindir],
			           libraries=['pyxcore', 'user32', 'Gdi32'])

tools_module = Extension('pyxie.devtool._pyxietools', 
                       sources=['pythonTools.cpp'],
                       include_dirs=['bin/include', numpy.get_include()],
			           library_dirs=[bindir],
			           libraries=['pyxtools','pyxcore'])

setup(name='pyxie', version='0.3.110',
		description='pyxie game engine module',
		author=u'Kiharu Shishikura',
		author_email='shishi@indigames.net',
        packages=find_packages(),
		ext_modules=[pyxie_module, tools_module],
		long_description=open('README.md').read(),
		license='MIT',
		install_requires=[
			'pyvmath', 'requests', 'numpy'
			],
		classifiers=[
			'Intended Audience :: Developers',
			'License :: OSI Approved :: MIT License',
			'Programming Language :: Python :: 3',
			#'Operating System :: MacOS :: MacOS X',
			#'Operating System :: POSIX :: Linux',
			'Operating System :: Microsoft :: Windows',
			'Topic :: Games/Entertainment',
		],
        data_files=[
            ('pyxie' ,  [bindir+"/pyxcore.dll"]),
            ('pyxie/devtool',  [bindir+"/pyxtools.dll",bindir+"/PVRTexLib.dll"])
        ],
        include_package_data=True
      )



