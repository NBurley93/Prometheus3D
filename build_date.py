import string
import datetime

# Open a file
fo = open("src/builddate.h", "r+")
print ("Getting build time/date...")
line = fo.readlines()
dnow = datetime.datetime.now();
for idx, item in enumerate(line):
    if 'BUILD_DT' in item:
        tmp_str =  item.split();
        timestr= dnow.strftime("%b-%d-%y-%I:%M%p");
        tmp_str[2] = "\""+timestr+"\"\n";
        item = ' '.join(tmp_str);
        #print item;
        line[idx] = item;    
fo.seek(0)

for idx, item in enumerate(line):
    fo.write(item)
fo.truncate()

# Close opend file
print ("Build time/date is now ", tmp_str[2])
fo.close()
exit(0)