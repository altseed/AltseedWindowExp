import aceutils

path = r'https://github.com/glfw/glfw/archive/'
zipname = '7a8516d296c40402bd64f29ad1800ebd98483a07.zip'
pathname = 'glfw-7a8516d296c40402bd64f29ad1800ebd98483a07'

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

aceutils.mkdir('../include')
aceutils.mkdir('../lib')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rm(zipname)
	aceutils.rmdir(pathname)
	aceutils.rmdir(r"glfw_bin")
	aceutils.rmdir(r"glfw_bin_x64")

	aceutils.wget(path + zipname)
	aceutils.unzip(zipname)
	aceutils.editCmakeForACE(pathname + r'/CMakeLists.txt')
	aceutils.mkdir(r"glfw_bin")
	aceutils.mkdir(r"glfw_bin_x64")

	with aceutils.CurrentDir('glfw_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../' + pathname + '/')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../Dev "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../' + pathname + '/')
			aceutils.call(r'make install')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../' + pathname + '/')
			aceutils.call(r'make install')

	with aceutils.CurrentDir('glfw_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../' + pathname + '/')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Release')
	
	aceutils.mkdir(r"../include/GLFW/")

	aceutils.copy(pathname + r'/include/GLFW/glfw3.h', r'../include/GLFW/')
	aceutils.copy(pathname + r'/include/GLFW/glfw3native.h', r'../include/GLFW')

	if aceutils.isWin():

		aceutils.mkdir(r'../lib/x86/')
		aceutils.mkdir(r'../lib/x86/Debug')
		aceutils.mkdir(r'../lib/x86/Release')

		aceutils.mkdir(r'../lib/x64/')
		aceutils.mkdir(r'../lib/x64/Debug')
		aceutils.mkdir(r'../lib/x64/Release')

		aceutils.copy(r'glfw_bin/src/Debug/glfw3.lib', r'../lib/x86/Debug/')
		aceutils.copy(r'glfw_bin/src/Release/glfw3.lib', r'../lib/x86/Release/')

		aceutils.copy(r'glfw_bin_x64/src/Debug/glfw3.lib', r'../lib/x64/Debug/')
		aceutils.copy(r'glfw_bin_x64/src/Release/glfw3.lib', r'../lib/x64/Release/')

	else:
		aceutils.copy(r'glfw_bin/src/libglfw3.a', r'../lib')
