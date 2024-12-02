import re


def flatten_list(lst):
    for elem in lst:
        if isinstance(elem, list):
            yield from flatten_list(elem)
        else:
            yield elem


class Tree(object):
    def __init__(self, root_name, data=None):
        self.root_node = Node(root_name, parent=None, data=data)
        self.node_pointer = self.root_node

    def add_data(self, data):
        self.node_pointer.add_data(data)

    def add_child(self, node_name, data=None):
        self.node_pointer.add_child(node_name, data=data)

    def print(self, indentation=""):
        print(indentation + self.root_node.node_name)
        for node in self.root_node.children:
            node.print(indentation + "    ")

    def get_node(self):
        return self.node_pointer

    def get_nodes(self):
        res = [self.root_node.node_name]
        for node in self.root_node.children:
            res.append(node.get_nodes())
        return res

    def cd(self, dir_name):
        if dir_name == "/":
            self.node_pointer = self.root_node
            return self
        if dir_name == "..":
            self.node_pointer = self.node_pointer.parent
            return self
        assert dir_name in [node.node_name for node in self.node_pointer.children]
        for node_child in self.node_pointer.children:
            if node_child.node_name == dir_name:
                self.node_pointer = node_child
                return self

    def calc_tree_size(self):
        self.root_node.process_data_size()
        self.root_node.process_node_size()

    def get_dir_sizes(self):
        res = [(self.root_node.node_name, self.root_node.node_size)]
        for node in self.root_node.children:
            res.append(node.get_dir_sizes())
        return res


class Node(object):
    def __init__(self, node_name, parent, data=None):
        self.node_name = node_name
        self.parent = parent
        self.children = []
        if data is not None:
            self.data = data
        else:
            self.data = []
        self.data_size = 0
        self.node_size = 0

    def add_data(self, data):
        self.data.append(data)

    def add_child(self, node_name, data=None):
        node = Node(node_name, parent=self, data=data)
        self.children.append(node)

    def get_nodes(self):
        res = [self.node_name]
        for node in self.children:
            res.append(node.get_nodes())
        return res

    def print(self, indentation):
        print(indentation + self.node_name)
        for node in self.children:
            node.print(indentation + "    ")

    def process_data_size(self):
        for data in self.data:
            self.data_size += int(data[0])
        for node in self.children:
            node.process_data_size()

    def process_node_size(self):
        if not len(self.children):
            self.node_size = self.data_size
        else:
            for node in self.children:
                node.process_node_size()
            self.node_size = self.data_size + sum(
                [node.node_size for node in self.children]
            )

    def get_dir_sizes(self):
        res = [(self.node_name, self.node_size)]
        for node in self.children:
            res.append(node.get_dir_sizes())
        return res


input = open("input.txt", "r").read().strip().split("\n")

tree = Tree("/")
for line in input:
    if re.match(r"\$ cd", line):
        folder_name = re.search(r"\$ cd (.+)", line).group(1)
        tree.cd(folder_name)
    if re.match(r"dir", line):
        dir_name = re.search(r"dir (.+)", line).group(1)
        tree.add_child(dir_name)
    if re.match(r"[^\$|dir]", line):
        file_tuple = re.search(r"(\w*) (\w*\.?\w*)", line).groups()
        tree.add_data(file_tuple)

tree.calc_tree_size()
# tree.print()

# Parte 1
list_of_dirs_sizes = list(flatten_list(tree.get_dir_sizes()))
list_smol_dirs = filter(lambda dir_size: dir_size[1] <= 100000, list_of_dirs_sizes)
print(sum([dir[1] for dir in list_smol_dirs]))

# Parte 2
root_size = tree.cd("/").get_node().node_size
unused_space = 70000000 - root_size
space_to_free = 30000000 - unused_space
dir_to_del = root_size
for dir in list_of_dirs_sizes:
    val = dir[1] - space_to_free
    if (val >= 0) and (dir[1] <= dir_to_del):
        dir_to_del = dir[1]
print(dir_to_del)
