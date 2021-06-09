import panacea.read_restart
import panacea.read_files

import numpy as np

# Class provides a means of reading multiple restart files
class RestartFileGroup:
    def __init__(self, path_to_files, base, extension):
        self.__file_grp_reader = panacea.read_files.ReadFiles(
            path_to_files, base, extension, panacea.read_restart.RestartFile
        )
        self.__file_name_to_index = {}

    def read(self, index_start=0, index_end=None):
        self.__file_grp_reader.read(index_start, index_end)

        first_file = True
        for index, file_name in enumerate(self.__file_grp_reader.files_read):
            self.__file_name_to_index[index] = file_name
            dims, dims = self.__file_grp_reader[file_name].covariance.shape
            if not first_file:
                if dims != self.__dims:

                    raise Exception(
                        "Different numbers of dimensions detected while "
                        "reading in a group of restart files. Inconsistency in the "
                        "dimensions is problematic."
                    )
            else:
                first_file = False
                self.__dims = dims

    def __getitem__(self, file_name_or_index):
        return self.__file_grp_reader[file_name_or_index]

    def getCovariance(self, file_name):
        if not self.__file_grp_reader.exists(file_name):
            return None
        return self.__file_grp_reader[file_name].covariance

    def getCovarianceMean(self, file_name):
        if not self.__file_grp_reader.exists(file_name):
            return None
        return self.__file_grp_reader[file_name].covariance_mean

    @property
    def indices_files_read(self):
        return self.__file_grp_reader.indices_files_read

    @property
    def files_read(self):
        return self.__file_grp_reader.files_read

    @property
    def dimensions(self):
        if self.__dims is None:
            raise Exception("Dims unknown, was a file read?")
        return self.__dims

    def getKernelCenters(self, file_name):
        if not self.__file_grp_reader.exists(file_name):
            return None
        return self.__file_grp_reader[file_name].kernel_centers

    def getNormalizationCoefficients(self, file_name):
        if not self.__file_grp_reader.exists(file_name):
            return None
        return self.__file_grp_reader[file_name].normalization_coefficients
