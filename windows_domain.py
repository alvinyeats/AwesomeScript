"""
windows域相关操作

python3.7
"""

import subprocess


class Domain(object):
    def __init__(self):
        self.accounts = []

    def obtain_accounts(self):
        # 获取域账号列表
        result = subprocess.Popen('net users /domain', shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        result_lines = result.stdout.readlines()
        for line in result_lines:
            line = line.decode('gbk').strip()
            # windows自带的输出提示里也有空格（1个），这些是我们不需要的，域账号之间的空格超过两个，
            # 故用双空格来判断是否是我们想要的输出行
            if '  ' in line:
                line_domains = [d for d in line.split() if d]
                self.accounts += line_domains

    def get_accounts(self):
        return self.accounts

    def print_account(self):
        print("domain accounts: ")
        print(self.accounts)


if __name__ == '__main__':
    ac = Domain()
    ac.obtain_accounts()
    ac.print_account()

