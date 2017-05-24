import string

# Open a file
fo = open("src/buildnum.h", "r+")
print ("Incrementing build number...")
line = fo.readlines()
for idx, item in enumerate(line):
   if 'BUILD_NUM' in item:
       tmp_str =  item.split();
       tmp_str[2] = str(int(tmp_str[2])+1)+"\n";
       item = ' '.join(tmp_str);
       #print item;
       line[idx] = item;    
fo.seek(0)

for idx, item in enumerate(line):
    fo.write(item)
fo.truncate()

# Close opend file
print ("Build number is now ", tmp_str[2])
fo.close()
exit(0)