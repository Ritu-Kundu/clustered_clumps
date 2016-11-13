import string
import random
import math

FOLDER = './experiments/'
FOLDERA = 'searchA/'
FOLDERB = 'searchB/'
FOLDERC = 'searchC/'

def gen_pos(d,a_bet):
    # d is 1 (for non-degen) or higher (upto n)
    n = len(a_bet)
    d_s = random.sample(range(n),d)
    pos = ['0']*n
    for i in range(n):
        if i in d_s:
            pos[i] = '1'
    return ''.join(pos)


def printable_pos(pos,a_bet):
    s = ''
    for i in range(len(pos)):
        if pos[i] == '1':
            s += a_bet[i]
    if len(s) > 1:
        s = '['+s+']'
    return s

def gen_pattern(size,k,a_bet):
    n = len(a_bet)
    d_s = random.sample(range(size),k) # degenerate indices 
    ps = []
    for i in range(size):
        if i in d_s:
            d = random.randrange(2,n)
        else:
            d = 1
        ps.append(gen_pos(d, a_bet))
    return ps

def printable_pattern(pat,a_bet):
    s = ''
    for p in pat:
        s += printable_pos(p,a_bet)
    return s

####not used###################################################
def partition_size(size, n):
    sizes = []
    rem = size
    for i in range(n-1):
        u = round(rem/2)
        s = random.randrange(1,u)
        rem -= s
        sizes.append(s)
    sizes.append(rem)
    return sizes

def partition_degen(sizes, s_deg):
    rem = s_deg
    n = len(sizes)
    ds = [0]*n
    while (rem>0):
        for i in range(n):
            d = random.randrange(max(sizes[i]-ds[i],rem)+1)
            ds[i] += d
            rem -= d
    return ds
#################################################################


def gen_patterns(size, num_pat, total_d, a_bet):
    d = random.sample(range(size), total_d)
    n = random.sample(range(1,size-1), num_pat-1)
    d_s = sorted(d)
    n_s = [0] + sorted(n) + [size]
 
    ds = [0]*num_pat
    ss = [0]*num_pat

    for i in range(1,num_pat+1):
        ss[i-1] = n_s[i]-n_s[i-1]

    nxt = 1
    for ind in d_s:
        if ind < n_s[nxt]:
            ds[nxt-1] += 1
        else:
            while ind >= n_s[nxt]:
                nxt += 1
            ds[nxt-1] += 1

    pss = []
   
    for i in range(num_pat):
        pss.append(gen_pattern(ss[i],ds[i],a_bet))
    return pss

def write_p_file(a_bet, ps, file_name):

    p_file = open(file_name,'w')
    
    p_file.write(a_bet)
    p_file.write('\n')
    
    for i in range(len(ps)):
        p_file.write('>')
        p_file.write(printable_pattern(ps[i],a_bet))
        p_file.write('\n')
        for pos in ps[i]:
            p_file.write(pos)
            p_file.write('\n')
        p_file.write('\n')
            

def write_t_file(size, a_bet,file_name):
    txt = ''.join(random.choice(a_bet) for i in range(size))
    t_file = open(file_name,'w')
    t_file.write(txt)


def single_run(s_sig,n_pats,s_pat_l, total_d, s_txt,i, search_type):

    sig = string.ascii_uppercase[0:s_sig] ## the alphabet string
    d = 0
    file_name = FOLDER
    if search_type == 'A':
        file_name += FOLDERA
        d = total_d
        write_t_file(s_txt,sig,file_name+'t'+str(i)+'.txt')
    elif search_type == 'B':
        file_name += FOLDERB
        d = 0
        ts = gen_patterns(s_txt, 1, total_d, sig)
        write_p_file(sig, ts,file_name+'t'+str(i)+'.txt')
    else:
        file_name += FOLDERC
        d = random.choice(range(1,total_d))
        ts = gen_patterns(s_txt, 1, total_d-d, sig)
        write_p_file(sig, ts,file_name+'t'+str(i)+'.txt')

    ps = gen_patterns(s_pat_l, n_pats, d, sig)
    write_p_file(sig, ps,file_name+'p'+str(i)+'.txt')
    

    #print('Success! Wrote files p.txt and t.txt')


def main():
    
    s_sig = 4
    text_size = [1000, 2000, 4000, 8000, 16000, 32000, 64000]
    pattern_size = [10,80, 160, 320, 640] 
    num_patterns = [2,20,40, 80, 160]
    d = [5, 10, 20, 40, 80]

    i=1
    for t in range(7):
        s_txt = text_size[t]
        for j in range(5):
            s_pat = pattern_size[j]
            n_pats =  num_patterns[j]
            nd = d[j]
            single_run(s_sig,n_pats,s_pat, nd, s_txt, i, 'A')
            single_run(s_sig,n_pats,s_pat, nd, s_txt, i, 'B')
            single_run(s_sig,n_pats,s_pat, nd, s_txt, i, 'C')
            i +=1
            
   

main()
