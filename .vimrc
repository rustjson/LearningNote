set cursorline
set nocompatible
set smartindent
set tabstop=4
set shiftwidth=4
set noexpandtab
set number
set fdm=marker
set paste
set hlsearch


set fileencodings=utf8,gb2312,gdk,gb18030
set termencoding=utf8
set encoding=prc

execute pathogen#infect()

set nocompatible
filetype off

set rtp+=~/.vim/bundle/Vundle.vim

call vundle#begin()

Plugin 'VundleVim/Vundle.vim'
Plugin 'Valloric/YouCompleteMe'
Plugin 'rust-lang/rust.vim'
Plugin 'scrooloose/nerdtree'

Plugin 'jistr/vim-nerdtree-tabs'
Plugin 'c.vim'
Plugin 'php.vim'
Plugin 'shawncplus/phpcomplete.vim'
Plugin 'scrooloose/syntastic'
Plugin 'xolox/vim-easytags'
Plugin 'xolox/vim-misc'

call vundle#end()
filetype plugin indent on

autocmd vimenter * NERDTree
autocmd VimEnter * wincmd p
"autocmd VimEnter * NERDTreeTabsOpen
autocmd BufEnter * NERDTreeMirror


let g:NERDTreeDirArrows = 0
autocmd StdinReadPre * let s:std_in=1
"autocmd VimEnter * if argc() == 0 && !exists("s:std_in") | NERDTree | endif
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTreeType") && b:NERDTreeType == "primary") | q | endif
"let NERDTreeMapOpenInTab='<ENTER>'
let g:nerdtree_tabs_open_on_console_startup=1

let g:ycm_global_ycm_extra_conf = "~/.vim/.ycm_extra_conf.py"
let g:ycm_key_list_select_completion=[]
let g:ycm_key_list_previous_completion=[]
let g:ycm_filetype_whitelist = { 'cpp': 1, 'c': 1, 'python':1 }

set statusline+=%#warningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*

let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0

let g:phpcomplete_add_class_extensions = ['mongo', 'yaf']
let g:phpcomplete_add_function_extensions = ['mongo', 'yaf']

let g:Powerline_symbols = 'fancy'

map <C-\> :tab split<CR>:exec("tag ".expand("<cword>"))<CR>
map <A-]> :vsp <CR>:exec("tag ".expand("<cword>"))<CR>

let g:esaytags_cmd = '/usr/bin/ctags'
let g:easytags_languages = {
\   'PHP': {
\     'cmd': '/usr/bin/ctags',
\       'args': [],
\       'fileoutput_opt': '-f',
\       'stdout_opt': '-f-',
\       'recurse_flag': '-R'
\   }
\}
let g:easytags_file = '~/.vim/tags'
let g:easytags_auto_highlight = 0
set tags=./tags;
let g:easytags_dynamic_files = 2
