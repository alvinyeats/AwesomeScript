import os


class Searcher(object):

    def __init__(self):
        self.root = ''
        self.files = []

    def set_root(self, dir_name):
        self.root = dir_name

    def search(self, file_type):
        """
        search files by file type, for example:
        str args: '.txt'
        tuple args: ('.txt', '.docx')
        :param file_type:
        :return:
        """
        for root, dirs, files in os.walk(self.root):
            for file in files:
                if file.endswith(file_type):
                    self.files.append(os.path.join(root, file))

    def get_files(self):
        return self.files

    def print_files(self):
        print(self.files)


if __name__ == "__main__":
    s = Searcher()
    s.set_root(r'C:\Users\wyr13087\Desktop')
    s.search(('.txt', '.docx'))
    s.print_files()
