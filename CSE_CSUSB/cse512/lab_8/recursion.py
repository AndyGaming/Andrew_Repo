# Yazhuo Liu
# recursion.py

# flatten a nested list

def flatten(lst):
    if not type(lst) == list:
        return lst
    if lst == []:
        return lst
    elif not type(lst[0]) == list:
        return [lst[0]] + flatten(lst[1:])
    else:
        return flatten(lst[0]) + flatten(lst[1:])
    
