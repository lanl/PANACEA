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
                raise Exception("Unable to convert first field to int, field is {}, line is {}".format(word, line))
        elif index == 1:
            try:
                cols = int(word)
            except:
                raise Exception("Unable to convert second field to int, field is {}, line is {}".format(word, line))
        else:
            error_msg = "Only expected two fields for rows and columns"
            raise Exception(error_msg)
        index = index + 1
    return rows, cols

class RestartFile:

    # Initialization
    def __init__(self):
        self.__entropy_type = None
        self.__distribution_type = ""
        self.__prefactor = 0.0
        self.__correlated = False
        self.__kernel_count = "Single"
        self.__kernel_center_calculation = "None"
        self.__primitive_type = "Gaussian"
        self.__normalization_method = "None"

        self.__normalization_coefficients = np.zeros(0)
        self.__kernel_centers = np.zeros((0,0))
        self.__covariance_total_pts = 0
        self.__covariance = np.zeros((0,0))
        self.__covariance_norm_state = "Unnormalized"
        # Needed to update the covariance matrix
        self.__covariance_mean = np.zeros(0)
    # Getters
    @property
    def entropy_type(self):
        return self.__entropy_type

    @property
    def distribution_type(self):
        return self.__distribution_type

    @property
    def prefactor(self):
        return self.__prefactor

    @property
    def correlated(self):
        return self.__correlated

    @property
    def kernel_count(self):
        return self.__kernel_count

    @property
    def kernel_center_calculation(self):
        return self.__kernel_center_calculation

    @property
    def primitive_type(self):
        return self.__primitive_type

    @property
    def normalization_method(self):
        return self.__normalization_method

    @property
    def normalization_coefficients(self):
        return self.__normalization_coefficients

    @property
    def kernel_centers(self):
        return self.__kernel_centers

    @property
    def covariance_total_pts(self):
        return self.__covariance_total_pts

    @property
    def covariance(self):
        return self.__covariance

    @property
    def covariance_norm_state(self):
        return self.__covariance_norm_state

    @property
    def covariance_mean(self):
        return self.__covariance_mean

    # Private Methods
    def __readSectionEntropy(self, fid):
        line = fid.readline()
        if "Self" in line:
            self.__entropy_type = "Self"
        elif "Cross" in line:
            self.__entropy_type = "Cross"
        else:
            raise Exception("Unrecognized entropy type")

    def __readSectionDistribution(self, fid):
        line = fid.readline()
        if "Kernel" in line:
            self.__distribution_type = "Kernel"
        elif "Histogram" in line:
            self.__distribution_type = "Histogram"
        else:
            raise Exception("Unrecognized distribution")

    def __readSectionPrefactor(self, fid):
        line = fid.readline()
        try:
            self.__prefactor = float(line)
        except:
            print("Unable to convert expected prefactor value to float")

    def __readSectionKernelSpecifications(self, fid):
        # Correlation
        line = fid.readline()
        if "Uncorrelated" in line:
            self.__correlated = False
        elif "Correlated" in line:
            self.__correlated = True
        else:
            raise Exception("Unrecognized correlation state in line")

        # Kernel count
        line = fid.readline()
        if "OneToOne" in line:
            self.__kernel_count = "OneToOne"
        elif "Single" in line:
            self.__kernel_count = "Single"
        elif "Fixed" in line:
            self.__kernel_count = "Fixed"
        else:
            raise Exception("Unrecognized kernel count in line")

        # Kernel count
        line = fid.readline()
        print("Reading primitive type line: {}".format(line))
        if "GaussianLog" in line:
            self.__primitive_type = "GaussianLog"
        elif "Exponential" in line:
            self.__primitive_type = "Exponential"
        elif "Gaussian" in line:
            self.__primitive_type = "Gaussian"
        else:
            raise Exception("Unrecognized primitive type in line {}".format(line))

        # Kernel normalization method
        line = fid.readline()
        if "None" in line:
            self.__normalization_method = "None"
        elif "Variance" in line:
            self.__normalization_method = "Variance"
        else:
            raise Exception("Unrecognized normalization method in line")

        # Kernel memory can ignore
        fid.readline()
        # Kernel Center calculation
        line = fid.readline()
        if "None" in line:
            self.__kernel_center_calculation = "None"
        elif "Mean" in line:
            self.__kernel_center_calculation = "Mean"
        elif "Median" in line:
            self.__kernel_center_calculation = "Median"
        else:
            raise Exception("Unrecognized kernel center calculation in line")

    def __readSectionNormalizationMetaData(self, fid):
        # Algorithm type can ignore
        fid.readline()
        # number of coefficients
        line = fid.readline()
        try:
            self.__number_norm_coefficients = int(line)
        except:
            print("Unable to read number of normalization coefficients in as an")
            print(" integer.")

    def __readSectionNormalizationCoefficients(self, fid):
        self.__normalization_coefficients.resize(self.__number_norm_coefficients)

        for index in range(0,self.__number_norm_coefficients):
            line = fid.readline()
            try:
                self.__normalization_coefficients[index] = float(line)
            except:
                print("Unable to convert expected normalization coefficient to ")
                print(" floating point value.")

    def __readSectionKernel(self, fid):
        # Ignore
        fid.readline()
        # Ignore
        fid.readline()
        line = fid.readline()
        if "[Meta Data]" in line:

            line = fid.readline()
            if "[Total Number Points]" in line:
                # ignore total number of points is captured else where
                fid.readline()
                line = fid.readline()

            if self.__kernel_center_calculation == "Median":
                # ignore [Number Points Tracked Near Median]
                # and other median properties
                while "[Data]" not in line:
                    line = fid.readline()

            if "[Data]" in line:
                # Orientation of the rows and columns not important I hope
                fid.readline()
                # Rows and columns
                line = fid.readline()
                rows, cols = getRowsAndCols(line)
                self.__kernel_centers.resize(rows,cols)
                for row in range(0, rows):
                    line = fid.readline()
                    col = 0
                    words = line.split()
                    for word in words:
                        self.__kernel_centers[row, col] = float(word)
                        col = col + 1
            else:
                raise Except("Missing [Data] tag under [Meta Data] tag")

    def __readSectionCovariance(self, fid):
        # Total number of points used to calculate the covariance matrix
        line = fid.readline()
        self.__covariance_total_pts = int(line)
        # Blank line
        fid.readline()

        line = fid.readline()
        if "[Normalization State]" in line:
            line = fid.readline()
            if "Normalized" in line:
                self.__covariance_norm_state = "Normalized"
            elif "Unnormalized" in line:
                self.__covariance_norm_state = "Unnormalized"
            else:
                raise Exception("Covariance normalization state unrecognized")

            # blank line
            fid.readline()
            # Matrix Type
            fid.readline()
            # Actual Type
            fid.readline()
            # blank line
            fid.readline()

            line = fid.readline()
            if "[Matrix]" in line:
                line = fid.readline()
                rows, cols = getRowsAndCols(line)
                self.__covariance.resize((rows,cols))
                for row in range(0, rows):
                    line = fid.readline()
                    words = line.split()
                    col = 0
                    for word in words:
                        try:
                            self.__covariance[row,col] = float(word)
                        except:
                            raise Exception("Unable to convert field {} to float, at line {}"\
                                    ", row {} and col {}".format(word,line,row,col))
                        col = col + 1

            # Blank line
            fid.readline()
            # [Vector Type] tag
            fid.readline()
            # Actual type
            fid.readline()
            # Direction
            fid.readline()
            # Blank line
            fid.readline()

            # The mean of the covariance matrix which is used to update the
            # covariance matrix
            line = fid.readline()
            if not "[Vector]" in line:
                error_msg = "Expected [Vector] tag while reading covariance "
                error_msg = error_msg + "section."
                raise Exception(error_msg)

            # rows and columns
            line = fid.readline()
            rows, cols = getRowsAndCols(line)
            self.__covariance_mean.resize(rows)

            for row in range(0,rows):
                line = fid.readline()
                self.__covariance_mean[row] = float(line)

    def read(self,file_name):
        with open(file_name,"r") as file1:
            for line in file1:
                # Check for heading
                if "[Entropy]" in line:
                    self.__readSectionEntropy(file1)
                elif "[Distribution]" in line:
                    self.__readSectionDistribution(file1)
                elif "[Prefactor]" in line:
                    self.__readSectionPrefactor(file1)
                elif "[Kernel Specifications]" in line:
                    self.__readSectionKernelSpecifications(file1)
                elif "[Normalization]" in line:
                    self.__readSectionNormalizationMetaData(file1)
                elif "[Normalization Coefficients]" in line:
                    self.__readSectionNormalizationCoefficients(file1)
                elif "[Kernel]" in line:
                    self.__readSectionKernel(file1)
                elif "[Covariance]" in line:
                    self.__readSectionCovariance(file1)

