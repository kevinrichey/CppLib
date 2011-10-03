
set makeprg=ruby1.9.1\ build.rb
map <F5> :make<CR>
map <F9> :!bin/test >log.txt<CR>:lf log.txt<CR>:lopen 6<CR>

set foldmethod=syntax

