# 在jupyter里输入东西来比

# 一共几个数字
def total_number(numbers_str):

    numbers_list = list(map(int, numbers_str.split()))
    print(len(numbers_list))
    
# 一共几个不重复的数字
def not_repeat_total_number(numbers_str):
    numbers_list = list(map(int, numbers_str.split()))
    temp_list = []
    for num in numbers_list:
        if num not in temp_list:
            temp_list.append(num)
    print(len(temp_list))
    
# 有哪些重复数字，以及重复次数；按升序输出
def get_repeat(numbers_str):
    # 将字符串转换为数字列表
    numbers_list = list(map(int, numbers_str.split()))
    # 使用字典统计每个数字出现的次数
    temp_dict = {}
    for num in numbers_list:
        if num not in temp_dict:
            temp_dict[num] = 1
        else:
            temp_dict[num]+=1
    # 将重复数字按升序排序并输出        
    for num in sorted(temp_dict.keys()):
        if temp_dict[num] > 1:
            print(num, temp_dict[num], '\n')


# 用来比较 输入 和 中序 ， 丢给get_repeat后得到的字符串是否相同
def compare_strings(str1, str2):
    # 将两个字符串按换行符分割成行
    lines1 = str1.strip().splitlines()
    lines2 = str2.strip().splitlines()
    
    # 如果行数不相等，直接返回 False
    if len(lines1) != len(lines2):
        return False
    
    # 遍历每一行进行比较
    for line1, line2 in zip(lines1, lines2):
        # 检查每一行是否为空行
        if not line1.strip() or not line2.strip():
            continue
        
        # 将每行拆分成数字和出现次数
        try:
            num1, count1 = map(int, line1.split())
            num2, count2 = map(int, line2.split())
        except ValueError:
            # 如果无法解包为两个数字，跳过这一行或报错
            print(f"Skipping invalid line: {line1} or {line2}")
            continue
        
        # 如果数字或者出现次数不一致，返回 False
        if num1 != num2 or count1 != count2:
            return False
    
    # 如果所有比较都通过，返回 True
    return True