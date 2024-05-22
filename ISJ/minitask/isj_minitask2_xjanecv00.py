# minitask 2
# change the last du to DU
import re

pattern = re.compile(r'(?<!"(?=Tarzan[1234]"))Tarzan[1234]')
text = '"Tarzan1 and Jane": Now Tarzan2 says to Jane: "Tarzan3" and "John and Tarzan4"'

print(re.findall(pattern, text))
