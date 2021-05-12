
import glob
from os import path

def getIndex(file_name, base):
    base_plus_index = file_name.split(".",1)[0]
    return int(base_plus_index.split(base,1)[1])

# Designed to read in a group of files as long as they have a format similar to:
# file_base_name_number.extension
# E.g.
# descriptors1.txt
# descriptors2.txt
# etc...
# Must provide the directory the files are stored in and provide the base and extesion
# to the files that will be readin
# The class that is actually able to parse the file also needs to be provided
class ReadFiles:
    def __init__(self,path_to_files, base,extension, read_class):

        # Check if path exists
        if not path.exists(path_to_files):
            raise Exception("Provided path does not exist")

        if not path.isdir(path_to_files):
            raise Exception("Provided path is not a folder.")

        if extension[0] != '.':
            extension = '.' + extension

        if not path_to_files.endswith('/'):
            path_to_files = path_to_files + '/'

        # Seach for a list of files with the base and extension
        glob_str = "{}{}[0-9]{}".format(path_to_files,base,extension)
        potential_files = glob.glob(glob_str)

        # Ensure that the named contents are actually files
        self.__files = []
        for potential_file in potential_files:
            if path.isfile(potential_file):
                self.__files.append(potential_file)

        self.__files.sort()
        self.__base = base
        self.__extension = extension
        self.__read_class = read_class
        self.__file_readers = {}

    @property
    def base(self):
        return self.__base

    @property
    def extension(self):
        return self.__extension

    @property
    def full_file_paths(self):
        return self.__files

    @property
    def indices_files_read(self):
        indices = []
        for file_name in self.__file_readers:
            indices.append(getIndex(file_name,self.__base))
        return indices

    # The names of the files that were found in the specified folder
    @property
    def available_files(self):
        file_base_names = []
        for file_path in self.__files:
            file_base_names.append(path.basename(file_path))
        return file_base_names

    # The names of the files that were found in the specified folder
    @property
    def files_read(self):
        file_base_names = []
        for file_name in self.__file_readers.keys():
            file_base_names.append(file_name)
        return file_base_names


    # index_start is inclusive
    # index_end is exclusive
    def read(self, index_start=0, index_end=None):
        # Ensure we are starting we a clean slate

        self.__file_readers = {}
        for file_in in self.__files:
            file_in_base = path.basename(file_in)
            index_file_in = int(file_in_base.replace(self.__base,'').replace(self.__extension,''))
            if index_file_in >= index_start:
                if index_end != None:
                    if index_file_in < index_end:
                        read_file = self.__read_class()
                        read_file.read(file_in)
                        self.__file_readers[file_in_base] = read_file
                else:
                    read_file = self.__read_class()
                    read_file.read(file_in)
                    self.__file_readers[file_in_base] = read_file

    # Was the file indicated with file_name read
    def exists(self, file_name_or_index):

        if isinstance(file_name_or_index,int):
            file_name = self.base + str(file_name_or_index) + self.extension
        else:
            file_name = file_name_or_index

        if file_name in self.__file_readers.keys():
            return True
        return False

    # Can pass in the index of the file as it appears in the file name
    # or can pass in the full file name
    def __getitem__(self, file_name_or_index):

        if isinstance(file_name_or_index,int):
            file_name = self.base + str(file_name_or_index) + self.extension
        else:
            file_name = file_name_or_index

        if file_name in self.__file_readers.keys():
            return self.__file_readers.get(file_name)
        return None

