
def total_number(numbers_str):

    numbers_list = list(map(int, numbers_str.split()))
    print(len(numbers_list))
    

def not_repeat_total_number(numbers_str):
    numbers_list = list(map(int, numbers_str.split()))
    temp_list = []
    for num in numbers_list:
        if num not in temp_list:
            temp_list.append(num)
    print(len(temp_list))
    
    
def get_repeat(numbers_str):
    numbers_list = list(map(int, numbers_str.split()))
    temp_dict = {}
    for num in numbers_list:
        if num not in temp_dict:
            temp_dict[num] = 1
        else:
            temp_dict[num]+=1
    for num in temp_dict:
        if temp_dict[num]>1:
            print(num, temp_dict[num],'\n')
            