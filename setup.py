from distutils.core import setup, Extension

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
                       include_dirs=['bin/include'],
			           library_dirs=['bin/win32'],
			           libraries=['pyxcore', 'user32', 'Gdi32'])

tools_module = Extension('pyxie.devtool._pyxietools', 
                       sources=['pythonTools.cpp'],
                       include_dirs=['bin/include'],
			           library_dirs=['bin/win32'],
			           libraries=['pyxtools','pyxcore'])

setup(name='pyxie', version='0.2.05',
		description='pyxie game engine module',
		author=u'Kiharu Shishikura',
		author_email='shishi@indigames.net',
		packages=['pyxie','pyxie.devtool','pyxie.apputil'],
		ext_modules=[pyxie_module, tools_module],
		long_description=open('README.md').read(),
		license='MIT',
		install_requires=[
			'pyvmath', 
			'requests'
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
		data_files = {
			'bin/win32/pyxcore.dll':['bin/win32/pyxcore.dll'],
			'bin/win32/pyxtools.dll':['bin/win32/pyxtools.dll'],
			'bin/win32/PVRTexLib.dll':['bin/win32/PVRTexLib.dll']
			}
      )
