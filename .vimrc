syntax on   " 设置语法高亮
set nu	    " 设置显示行号,或 set number 
set tabstop=4	" 设置一个缩进占4个空格
set smartindent	" 设置自动缩进
set mouse=a	" 设置鼠标一直可用,set mouse=(空)取消
"set list lcs=tab:\|\	" tab提示
"set cc=120	" 设置第120列高亮, set cc=0取消
"set cursorline	" 突出显示当前行
colorscheme desert  " 配色主题
set backspace=2
set hlsearch	" 查找高亮

" bundle 插件管理器配置
set nocompatible
filetype off

" bundle目录
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

Plugin 'VundleVim/Vundle.vim'
Plugin 'scrooloose/nerdtree'
Plugin 'scrooloose/syntastic'
Plugin 'Yggdroot/indentLine'
Plugin 'majutsushi/tagbar'
Plugin 'kien/ctrlp.vim'
Plugin 'Valloric/YouCompleteMe'
Plugin 'vim-airline/vim-airline'
Plugin 'vim-airline/vim-airline-themes'

call vundle#end()
filetype plugin indent on

" nerdtree
"autocmd vimenter * NERDTree
" 设置 NERDTree 子窗口宽度
let NERDTreeWinSize=30
" 设置 NERDTree 子窗口位置
let NERDTreeWinPos="left"
" 显示隐藏文件
let NERDTreeShowHidden=0

map <F8> :NERDTreeMirror<CR> " 通过F3 打开/关闭 nerdtree
map <F8> :NERDTreeToggle<CR> 

" 自动补全配置
set completeopt=longest,menu	"让Vim的补全菜单行为与一般IDE一致(参考VimTip1228)
autocmd InsertLeave * if pumvisible() == 0|pclose|endif	"离开插入模式后自动关闭预览窗口
inoremap <expr> <CR>       pumvisible() ? "\<C-y>" : "\<CR>"	"回车即选中当前项
"上下左右键的行为 会显示其他信息
inoremap <expr> <Down>     pumvisible() ? "\<C-n>" : "\<Down>"
inoremap <expr> <Up>       pumvisible() ? "\<C-p>" : "\<Up>"
inoremap <expr> <PageDown> pumvisible() ? "\<PageDown>\<C-p>\<C-n>" : "\<PageDown>"
inoremap <expr> <PageUp>   pumvisible() ? "\<PageUp>\<C-p>\<C-n>" : "\<PageUp>"

"youcompleteme  默认tab  s-tab 和自动补全冲突
"let g:ycm_key_list_select_completion=['<c-n>']
let g:ycm_key_list_select_completion = ['<Down>']
"let g:ycm_key_list_previous_completion=['<c-p>']
let g:ycm_key_list_previous_completion = ['<Up>']
let g:ycm_confirm_extra_conf=0 "关闭加载.ycm_extra_conf.py提示

let g:ycm_collect_identifiers_from_tags_files=1	" 开启 YCM 基于标签引擎
let g:ycm_min_num_of_chars_for_completion=2	" 从第2个键入字符就开始罗列匹配项
let g:ycm_cache_omnifunc=0	" 禁止缓存匹配项,每次都重新生成匹配项
let g:ycm_seed_identifiers_with_syntax=1	" 语法关键字补全
nnoremap <F5> :YcmForceCompileAndDiagnostics<CR>	"force recomile with syntastic
"nnoremap <leader>lo :lopen<CR>	"open locationlist
"nnoremap <leader>lc :lclose<CR>	"close locationlist
inoremap <leader><leader> <C-x><C-o>
"在注释输入中也能补全
let g:ycm_complete_in_comments = 1
"在字符串输入中也能补全
let g:ycm_complete_in_strings = 1
"注释和字符串中的文字也会被收入补全
let g:ycm_collect_identifiers_from_comments_and_strings = 0


" ctags
set tags=./tags,./TAGS,tags;~,TAGS;~

set cscopetag
set csto=0

if filereadable("cscope.out")
    cs add cscope.out
elseif $CSCOPE_DB != ""
    cs add $CSCOPE_DB
endif

set cscopeverbose

nmap zs :cs find s <C-R>=expand("<cword>")<CR><CR>
nmap zg :cs find g <C-R>=expand("<cword>")<CR><CR>
nmap zc :cs find c <C-R>=expand("<cword>")<CR><CR>
nmap zt :cs find t <C-R>=expand("<cword>")<CR><CR>
nmap ze :cs find e <C-R>=expand("<cword>")<CR><CR>
nmap zf :cs find f <C-R>=expand("<cfile>")<CR><CR>
nmap zi :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
nmap zd :cs find d <C-R>=expand("<cword>")<CR><CR>

nmap <F4> :TlistToggle<CR><CR>
let Tlist_Show_One_File=1
let Tlist_Exit_OnlyWindow=1
let Tlist_Use_Right_Window=1	" 窗口右侧
set ut=100

" Tagbar
" 设置tagbar的宽度
let g:tagbar_width=60
let g:tagbar_autofocus=1
let g:tagbar_right=1
nmap <F7> :TagbarToggle<CR>

" 打开ctrlp搜索
let g:ctrlp_map = '<leader>ff'
let g:ctrlp_cmd = 'CtrlP'
" 相当于mru功能，show recently opened files
map <leader>fp :CtrlPMRU<CR>
"set wildignore+=*/tmp/*,*.so,*.swp,*.zip     " MacOSX/Linux"
let g:ctrlp_custom_ignore = {
    \ 'dir':  '\v[\/]\.(git|hg|svn|rvm)$',
    \ 'file': '\v\.(exe|so|dll|zip|tar|tar.gz)$',
    \ }
"\ 'link': 'SOME_BAD_SYMBOLIC_LINKS',
let g:ctrlp_working_path_mode=0
let g:ctrlp_match_window_bottom=1
let g:ctrlp_max_height=15
let g:ctrlp_match_window_reversed=0
let g:ctrlp_mruf_max=500
let g:ctrlp_follow_symlinks=1

set path+=/Users/caizh/github/linux/include
set path+=/Users/caizh/github/linux/arch/x86/include
set path+=/Users/caizh/github/glibc/include

" syntastics配置
set statusline+=%#warningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*

let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0
