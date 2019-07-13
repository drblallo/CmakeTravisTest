let CMAKE_TYPE = 0
let BUILD_DIRECTORY = "build"
let CPPCOMPILER = "g++" 
let CCOMPILER = "gcc"
let BUILD_TYPE = "Debug"
let EXTRA_CONFIG = "-DCMAKE_CXX_FLAGS=--coverage"
let GENERATOR = "Ninja"
let TARGET = "utils"

let g:ctrlp_custom_ignore = '\v[\/](release|build|gli|glbinding|googletest|glfw|glm)|(\.(swp|ico|git|svn|lock))$'

function! s:updateCmake()

	call AQAppend(":lcd " . g:BUILD_DIRECTORY)
	let l:t = AQAppend(s:getBuildCommand())
	call AQAppend(":lcd ../")
	call AQAppendOpen(0, l:t)
	call AQAppendCond("!cp " . g:BUILD_DIRECTORY . "/compile_commands.json ./", 1, l:t)
endfunction

function! s:setType(val, cmakeBuildBir, cCompiler, cppCompiler, buildType, extra, generator)
	let g:CMAKE_TYPE = a:val
	let g:BUILD_DIRECTORY = a:cmakeBuildBir
	let g:CCOMPILER = a:cCompiler
	let g:CPPCOMPILER = a:cppCompiler
	let g:BUILD_TYPE = a:buildType
	let g:EXTRA_CONFIG = a:extra
	let g:GENERATOR = a:generator

	call s:updateCmake()
endfunction

function! s:getBuildCommand()
	let s:command =  "!cmake -DCMAKE_BUILD_TYPE=" . g:BUILD_TYPE .  " -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_INSTALL_PREFIX=./install" . " -DCMAKE_C_COMPILER=" . g:CCOMPILER . " -DCMAKE_CXX_COMPILER=" . g:CPPCOMPILER . " -G " . g:GENERATOR . " " . g:EXTRA_CONFIG . " --build ../"
	return s:command
endfunction

function! s:Rebuild()
	call AQAppend("!rm -r ./" . g:BUILD_DIRECTORY)
	call AQAppend("!mkdir ./" . g:BUILD_DIRECTORY)
	call s:updateCmake()
endfunction

function! s:RunTest(param, executible, args)
	let l:t = s:SilentRun(a:param, a:executible, a:args)
	call AQAppendOpen(0, l:t[0])
	call AQAppendCond("call ParseClangOutput()", 0, l:t[0])

	call AQAppendOpen(-1, l:t[1])
	call AQAppendCond("call RunOnBuffer()", -1, l:t[1])
	call AQAppendCond("call ApplyTestSyntax()", -1, l:t[1])

	call AQAppend("setlocal nomodified")
endfunction

function! s:silentBuild(target)
	let s:build = "!cmake --build " . g:BUILD_DIRECTORY . " --target " . a:target . " -- -j 4"
	call AQAppendOpen(0)
	return AQAppend(s:build)
endfunction

function! s:SilentRun(target, executible, args)
	let s:exec = "!./" . g:BUILD_DIRECTORY . "/" . a:executible . " " . a:args
	let l:ret = s:silentBuild(a:target)
	return [l:ret, AQAppendCond(s:exec, 1, l:ret)]
endfunction

function! s:Run(param, executible, args)
	let l:r = s:SilentRun(a:param, a:executible, a:args)
	call AQAppendOpen(0, l:r[0])
	call AQAppendCond("call ParseClangOutput()", 0, l:r[0])

	call AQAppendOpen(-1, l:r[1])
	call AQAppendCond("setlocal nomodified")

	call AQAppendOpenError(0, l:r[1])
	call AQAppendCond("call AsanParseBuffer()", 0, l:r[1])

	call AQAppendCond("setlocal nomodified")
endfunction

function! s:RunD(target, executible, args)
	let s:exec = "./" . g:BUILD_DIRECTORY . "/" . a:executible . " " . a:args

	let l:t = s:silentBuild(a:target)
	call AQAppendCond("Termdebug -r --args " . s:exec, 1, l:t)

endfunction

function! s:goToTest(name)
	execute "vimgrep " . a:name . " ../" . g:TARGET . "/test/src/*.cpp"	. " **/" . g:TARGET . "/test/src/*.cpp"
endfunction

function! s:setTarget(name)
	let g:TARGE = a:name
endfunction

function! s:genDoc()
	let l:ret = s:silentBuild("doc")
	call AQAppendCond('QTBSearch ./build/doc_doxygen/html/index.html', 1, l:ret)
endfunction

function! s:genCoverage()
	let l:ret = s:silentBuild("all")
	let l:ret = s:silentBuild("test")
	let l:ret = s:silentBuild("coverage")
	call AQAppendCond('QTBSearch ./build/Coverage/index.html', 1, l:ret)
endfunction

command! -nargs=0 CMDEBUG call s:setType(0, "build", "gcc", "g++", "Debug", "-DCMAKE_CXX_FLAGS=--coverage", "Ninja")
command! -nargs=0 CMRELEASE call s:setType(1, "release", "clang", "clang++", "Release", "", "Ninja")
command! -nargs=0 CMASAN call s:setType(2, "release", "clang", "clang++", "Debug", "-DCMAKE_CXX_FLAGS=-fsanitize=address -DCMAKE_CXX_FLAGS=-fno-omit-frame-pointer", "Ninja")
command! -nargs=0 CMTSAN call s:setType(3, "build", g:CCLANG, g:CPPCLANG, "Debug", "-DCMAKE_CXX_FLAGS='-fsanitize=thread -O1'", "Ninja")

command! -nargs=0 REBUILD call s:Rebuild()
command! -nargs=0 TALL call s:RunTest(g:TARGET . "Test", g:TARGET . "/test/" . g:TARGET . "Test", "")
command! -nargs=0 TSUIT call s:RunTest(g:TARGET . "Test", g:TARGET . "/test/" . g:TARGET . "Test", GTestOption(1))
command! -nargs=0 TONE call s:RunTest(g:TARGET . "Test", g:TARGET . "/test/" . g:TARGET . "Test", GTestOption(0))
command! -nargs=0 RUN call s:Run("main", "main", "")
command! -nargs=0 DTALL call s:RunD(g:TARGET . "Test", g:TARGET . "/test/" . g:TARGET . "Test", "")
command! -nargs=0 DTSUIT call s:RunD(g:TARGET . "Test", g:TARGET . "/test/" . g:TARGET . "Test", GTestOption(1))
command! -nargs=0 DTONE call s:RunD(g:TARGET . "Test", g:TARGET . "/test/" . g:TARGET . "Test", GTestOption(0))
command! -nargs=0 DRUN call s:RunD("main", "main", "")
command! -nargs=0 GOTOTEST call s:goToTest(expand("<cword>"))
command! -nargs=1 SETTARGET call s:setTarget(<f-args>)
command! -nargs=0 DOC call s:genDoc()
command! -nargs=0 COVERAGE call s:genCoverage()

nnoremap <leader><leader>gt :vsp<cr>:GOTOTEST<cr>
nnoremap <leader><leader>b :REBUILD<cr>
nnoremap <leader><leader>r :RUN<cr>
nnoremap <leader><leader>dr :DRUN<cr>
nnoremap <leader><leader>ta :TALL<cr>
nnoremap <leader><leader>dta :DTALL<cr>
nnoremap <leader><leader>ts :TSUIT<cr>
nnoremap <leader><leader>dts :DTSUIT<cr>
nnoremap <leader><leader>to :TONE<cr>
nnoremap <leader><leader>dto :DTONE<cr>

command! -nargs=1 Rename call s:clangRename(<f-args>)

function! s:clangRename(newName)
	let s:offset = line2byte(line(".")) + col(".") - 2
	let command = "!clang-rename -offset=" . s:offset . " -i -new-name=" . a:newName . " " . expand('%:t')
	call AQAppend(command)
	call AQAppendCond("checktime", 1)
endfunction
