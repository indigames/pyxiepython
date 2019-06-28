from distutils.core import setup, Extension

pyxie_module = Extension('pyxie', 
                       sources=[
                           'Backyard.cpp',
                           'pythonEnvironment.cpp',
                           'pyxieFile.cpp',
                           'pythonEditableFigure.cpp',
                           'Window.cpp',
                           'pythonShowcase.cpp',
                           'pythonPlatform.cpp',
                           'pythonAnimator.cpp',
                           'pythonModule.cpp',
                           'pythonCamera.cpp',
                           'pythonShaderGenerator.cpp',
                           'pythonFigure.cpp',
                           'pythonResource.cpp'
                       ],
                       include_dirs=['pyxie/include'],
			           library_dirs=['pyxie/win32'],
			           libraries=['pyxcore', 'user32', 'Gdi32'])

tools_module = Extension('pyxietools', 
                       sources=['pythonTools.cpp'],
                       include_dirs=['pyxie/include'],
			           library_dirs=['pyxie/win32'],
			           libraries=['pyxtools','pyxcore'])

setup(name='pyxie', version='0.1.0',
      description='pyxie game engine core module',
      ext_modules=[pyxie_module, tools_module],
      data_files = {
      'pyxie/win32/pyxcore.dll':['pyxie/win32/pyxcore.dll'],
      'pyxie/win32/pyxtools.dll':['pyxie/win32/pyxtools.dll'],
      'pyxie/win32/PVRTexLib.dll':['pyxie/win32/PVRTexLib.dll']
      }
      )
