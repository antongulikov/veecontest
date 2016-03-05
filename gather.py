import sys

files = ["vertex.h", "city.h", "action.h", "person.h" , "outputer.h", "driver.h", "oracle.h", 
		"oracle.cpp", "stupidOracle.h", "stupidOracle.cpp", "main.cpp"]

program = "#include <bits/stdc++.h> \n\nusing namespace std;"

for file in files:
	file_name = "./" + file
	with open(file_name, "r") as fr:
		for line in fr.readlines():
			if line[0] != '#':
				program = program + line
print(program)
