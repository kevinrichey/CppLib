projectFile = ARGV[0]
outputFile = ARGV[1]

file = File.new(projectFile)
lines = file.readlines()
filesToCompile = lines.inject("") { |result,line| result + " " + line.strip }
system("g++ -std=c++0x #{filesToCompile} -pedantic `sdl-config --cflags --libs` -o bin/#{outputFile}")

