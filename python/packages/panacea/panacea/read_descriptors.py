#!/usr/bin/python3

import matplotlib.pyplot as plt
import numpy as np
import copy
import re

# Helper functions
def getRowsAndCols(line):
    index = 0
    words = line.split()
    for word in words:
        if index == 0:
            try:
                rows = int(word)
            except:
                raise Exception(
                    "Unable to convert first field to int, field is {}, line is {}".format(
                        word, line
                    )
                )
        elif index == 1:
            try:
                cols = int(word)
            except:
                raise Exception(
                    "Unable to convert second field to int, field is {}, line is {}".format(
                        word, line
                    )
                )
        else:
            error_msg = "Only expected two fields for rows and columns"
            raise Exception(error_msg)
        index = index + 1
    return rows, cols


class DescriptorFile:

    # Initialization
    def __init__(self):
        self.__descriptors = np.zeros((0, 0))

    # Getters
    @property
    def descriptors(self):
        return self.__descriptors

    @property
    def number_descriptors(self):
        return self.__descriptors.shape[0]

    @property
    def dimensions(self):
        return self.__descriptors.shape[1]

    def read(self, file_name):
        with open(file_name, "r") as file1:
            # Ignore first line
            file1.readline()
            # Ignore orientation
            file1.readline()
            line = file1.readline()
            rows, cols = getRowsAndCols(line)
            self.__descriptors.resize((rows, cols))
            for row in range(0, rows):
                line = file1.readline()
                words = line.split()
                col = 0
                for word in words:
                    try:
                        self.__descriptors[row, col] = float(word)
                    except:
                        raise Exception(
                            "Unable to convert field {} to float, at line {}"
                            ", row {} and col {}".format(word, line, row, col)
                        )
                    col = col + 1
