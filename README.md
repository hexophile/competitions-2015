# competitions-2015
#List of reasons for this repo: http://einstein.ms.polsl.pl/Files/Zadania2015.pdf

#First task is about encryption:

#Numeric system for beginning = 2
#To encrypt: "Word"

#First letter: 'W'
#'W' = 87(10) = 1010111(2) // This is our wanted result
#       └──────────┐
#Next letter: 'o'  ↓
#Numeric system = (87 % 8) + 2 = 7 + 2 = 9
#'o' = 111(10) = 133(9)←─────────────────┘
#       └──────────┐
#Next one: 'r'	  ↓
#Numeric system = (111 % 8) + 2 = 9 // Unfortunately
#'r' = 114(10) = 136(9)←──────────┘
#       └──────────┐
#Last letter: 'd'  ↓
#Numeric system = (114 % 8) + 2 = 4
#'d' = 100(10) = 1210(4)←─────────┘

#Final result: 1010111 133 136 1210

#Also algorithm have to decrypt.
