
set makeprg=ruby1.9.1\ build.rb\ project.txt
map <F9> :!bin/test >log.txt<CR>:lf log.txt<CR>:lopen 6<CR>
map <F5> :make<CR>

set foldmethod=syntax

