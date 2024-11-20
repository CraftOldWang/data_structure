def getsubsets(n):
    if n==1:
        return [[],['a']]
    else:
        presetofsub=getsubsets(n-1)
        presetofsub.remove([])
        for subset in presetofsub:
            for i in range(len(subset)):
               subset[i]=chr(ord(subset[i])+1)
        
        return [[],['a']]+[['a']+sub for sub in presetofsub]+presetofsub
    
    
# n=input()
# allsubsets=getsubsets(n)
# i=0
# while i<n:
#     print(allsubsets[i])
#     i+=1







