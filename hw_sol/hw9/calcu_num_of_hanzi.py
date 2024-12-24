import re

def count_chinese_characters(text):
    # 正则表达式匹配中文字符（汉字）
    chinese_characters = re.findall(r'[\u4e00-\u9fa5]', text)
    return len(chinese_characters)

# 测试字符串
long_text ="""

"""

# 统计中文字符数量
chinese_count = count_chinese_characters(long_text)
print(f"中文字符的数量是: {chinese_count}")
