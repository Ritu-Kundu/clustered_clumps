import string
import random
import math

FOLDER = './experiments/'

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


def gen_patterns(size_l,size_u,s_deg_l,s_deg_u, n, a_bet):
    pss = []
    for i in range(n):
        size = random.randrange(size_l,size_u)
        d = random.randrange(round(size*s_deg_l), round(size*s_deg_u))
        pss.append(gen_pattern(size,d,a_bet))
    return pss

def write_p_file(a_bet, ps,i):

    p_file = open(FOLDER+'p'+str(i)+'.txt','w')
    
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
            

def write_t_file(size, a_bet,i):
    txt = ''.join(random.choice(a_bet) for i in range(size))
    t_file = open(FOLDER+'t'+str(i)+'.txt','w')
    t_file.write(txt)


def single_run(s_sig,n_pats,s_pat_l,s_pat_u,s_deg_l,s_deg_u,s_txt,i):

    sig = string.ascii_uppercase[0:s_sig] ## the alphabet string
    
    ps = gen_patterns(s_pat_l,s_pat_u, s_deg_l,s_deg_u,n_pats, sig)

    write_p_file(sig, ps,i)
    write_t_file(s_txt,sig,i)

    #print('Success! Wrote files p.txt and t.txt')


def main():
    
    print('Random Pattern and Text Generation Utility')
    
    #s_sig = int(input('Enter the size of the Alphabet (between 1 and 26):'))

    #n_pats = int(input('Enter the number of Patterns (per run):'))

    s_pat_l = 5
    s_pat_u = 20
    print('Pattern length set between 5 and 20')

    s_deg_l = 0.1
    s_deg_u = 0.5
    print('Pattern degeneracy set between 10% and 50%')

    #s_pat = int(input('Enter the total size of the Patterns:'))

    #s_deg = int(input('Enter the total size of the degenerate positions:'))
    
    #s_txt = int(input('Enter the size of the Text:'))
    s_sig = 4
    text_size = [1000, 10000, 10000]
    num_patterns = [5, 10, 20, 40, 80, 160]
    i=0

    for s_txt in text_size:
        for n_pats in num_patterns:
            i = i+1
            single_run(s_sig,n_pats,s_pat_l,s_pat_u,s_deg_l,s_deg_u,s_txt,i)
            
    

    

main()
