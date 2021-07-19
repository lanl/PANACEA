import panacea.read_descriptors
import panacea.read_files

import numpy as np

# Class provides a means of reading multiple descriptor files
class DescriptorFileGroup:
    def __init__(self, path_to_files, base, extension):
        self.__file_grp_reader = panacea.read_files.ReadFiles(
            path_to_files, base, extension, panacea.read_descriptors.DescriptorFile
        )
        self.__min_vals = np.zeros((0, 0))
        self.__max_vals = np.zeros((0, 0))
        self.__dims = 0
        self.__total_descriptors = 0
        self.__file_name_to_index = {}

    def read(self, index_start=0, index_end=None):
        self.__file_grp_reader.read(index_start, index_end)

        first_file = True
        for index, file_name in enumerate(self.__file_grp_reader.files_read):
            self.__file_name_to_index[index] = file_name
            pts, dims = self.__file_grp_reader[file_name].descriptors.shape
            self.__total_descriptors = self.__total_descriptors + pts
            if not first_file:
                if dims != self.__dims:

                    raise Exception(
                        "Different numbers of dimensions detected while "
                        "reading in a group of descriptor files. Inconsistency in the "
                        "dimensions is problematic."
                    )
            else:
                first_file = False
                self.__dims = dims

        self.__min_vals.resize(len(self.__file_grp_reader.files_read), self.__dims)
        self.__max_vals.resize(len(self.__file_grp_reader.files_read), self.__dims)

        for index, file_name in enumerate(self.__file_grp_reader.files_read):
            self.__min_vals[index] = self.__file_grp_reader[file_name].descriptors.min(
                axis=0
            )
            self.__max_vals[index] = self.__file_grp_reader[file_name].descriptors.max(
                axis=0
            )

    def __getitem__(self, file_name_or_index):
        return self.__file_grp_reader[file_name_or_index]

    @property
    def minmax(self):
        return self.__min_vals.min(axis=0), self.__max_vals.max(axis=0)

    @property
    def dimensions(self):
        return self.__dims

    @property
    def indices_files_read(self):
        return self.__file_grp_reader.indices_files_read

    # All descriptors in a single numpy array
    @property
    def descriptors(self):
        all_descriptors = np.zeros((self.__total_descriptors, self.__dims))
        index = 0
        for file_name in self.__file_grp_reader.files_read:
            local_descriptors = self.__file_grp_reader[file_name].descriptors
            rows, cols = local_descriptors.shape
            for row in range(0, rows):
                for col in range(0, cols):
                    all_descriptors[index, col] = local_descriptors[row, col]
                index = index + 1
        return all_descriptors
