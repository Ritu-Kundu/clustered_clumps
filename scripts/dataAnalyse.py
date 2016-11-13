import re

FOLDER = './experiments/'
FOLDERA = 'searchA/'
FOLDERB = 'searchB/'
FOLDERC = 'searchC/'


def process_o_file(a_file_name, o_folder, n):
    a_file = open(a_file_name,'w')
    a_file.write("alphabet_size\t\t#patterns\t\tpattern-size\t\ttext-size\t\td\t\t#clustered-clumps\t\ttime(in sec)\n")
    for i in range(1,n+1):
        f = open(o_folder+'o'+str(i)+'.txt','r')
        for j in range(6):
            para, val = f.readline().split(':')
            num = int(val)
            a_file.write("%d\t\t" % num)
        # Read time
        dummy, tm = f.readline().split(':')
        tmf = re.findall("\d+\.\d+", tm)
        a_file.write("%s\n" % tmf[0])
        #res = res+tmf[0]+'\n'


def main():
    num = 35 
    process_o_file(FOLDER + "analysis_searchA.txt", FOLDER + FOLDERA, num)
    process_o_file(FOLDER + "analysis_searchB.txt", FOLDER + FOLDERB, num)
    process_o_file(FOLDER + "analysis_searchC.txt", FOLDER + FOLDERC, num)

main()
