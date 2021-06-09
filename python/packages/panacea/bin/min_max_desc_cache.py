#!/usr/bin/python3

# PANACEA imports
import panacea.descriptor_file_group

# standard imports
import argparse
import numpy as np
import os


class MinMaxDescCache:
    def __init__(self, file_name_path, num_bins=20):
        self.__name_path = file_name_path
        self.__min_vals = np.zeros(0)
        self.__max_vals = np.zeros(0)
        self.__num_bins = num_bins

    def __writeCache(self):
        print("Trying to write to {}".format(self.__name_path))

        if not os.path.exists(os.path.dirname(self.__name_path)):
            os.makedirs(os.path.dirname(self.__name_path))

        with open(self.__name_path, "w") as cache_fid:
            cache_fid.write("%d\n" % self.__ndim)
            for max_val in self.__max_vals:
                cache_fid.write("%e " % max_val)
            cache_fid.write("\n")
            for min_val in self.__min_vals:
                cache_fid.write("%e " % min_val)
            cache_fid.write("\n\n")
            cache_fid.write("vmax values for normalized iso plots\n")
            cache_fid.write(
                "Dim1 | Dim2 | Bin Width Dim1 | Bin Width Dim2 |     vmax\n"
            )
            index = 0
            for dim1 in range(0, self.__ndim - 1):
                for dim2 in range(1, self.__ndim):
                    if dim2 > dim1:
                        cache_fid.write(
                            "%5d  %5d  %15e  %15e  %14e\n"
                            % (
                                dim1,
                                dim2,
                                self.__bin_widths[dim1],
                                self.__bin_widths[dim2],
                                self.__den_val[index],
                            )
                        )
                        index = index + 1

    def __readCache(self):
        self.__min_vals = np.zeros(0)
        self.__max_vals = np.zeros(0)
        self.__den_val = np.zeros(0)
        self.__bin_widths = np.zeros(0)

        with open(self.__name_path, "r") as cache_fid:
            line_num = 0
            index = 0
            for line in cache_fid:
                if line_num == 0:
                    self.__ndim = int(line)
                    self.__min_vals = np.resize(self.__min_vals, self.__ndim)
                    self.__max_vals = np.resize(self.__max_vals, self.__ndim)
                    self.__den_val = np.resize(
                        self.__den_val,
                        int((self.__ndim * self.__ndim - self.__ndim) / 2.0),
                    )
                    self.__bin_widths = np.resize(self.__bin_widths, self.__ndim)
                elif line_num == 1:
                    words = line.split()
                    dim = 0
                    for word in words:
                        self.__max_vals[dim] = float(word)
                        dim = dim + 1
                elif line_num == 2:
                    words = line.split()
                    dim = 0
                    for word in words:
                        self.__min_vals[dim] = float(word)
                        dim = dim + 1
                elif line_num > 5 and line_num <= 5 + len(self.__den_val):
                    words = line.split()
                    self.__bin_widths[int(words[0])] = float(words[2])
                    self.__bin_widths[int(words[1])] = float(words[3])
                    self.__den_val[index] = float(words[4])
                    index = index + 1
                line_num = line_num + 1

    def createCache(self, all_descriptors):
        self.__min_vals = all_descriptors.min(axis=0)
        self.__max_vals = all_descriptors.max(axis=0)
        self.__ndim = all_descriptors.shape[1]
        self.__den_val = np.zeros(int((self.__ndim * self.__ndim - self.__ndim) / 2.0))
        self.__bin_widths = np.subtract(
            self.__max_vals.astype(float), self.__min_vals.astype(float)
        ) / float(self.__num_bins)

        # Check bin widths if any are 0 set equal to 1
        for index, bin_width in enumerate(self.__bin_widths):
            if bin_width == 0:
                print("Bin width of size 0 detected setting to value of 1/num_bins\n")
                self.__bin_widths[index] = 1.0 / float(self.__num_bins)

        print("Number of bins is %d" % self.__num_bins)
        print("Bin widths")
        print(self.__bin_widths)
        index = 0
        for dim1 in range(0, self.__ndim - 1):
            for dim2 in range(1, self.__ndim):
                if dim2 > dim1:
                    x = all_descriptors[:, dim2]
                    y = all_descriptors[:, dim1]
                    if self.__min_vals[dim2] != self.__max_vals[dim2]:
                        x_edges = np.arange(
                            self.__min_vals[dim2],
                            self.__min_vals[dim2]
                            + float(self.__num_bins) * self.__bin_widths[dim2],
                            self.__bin_widths[dim2],
                        )
                    else:
                        x_edges = np.arange(
                            self.__min_vals[dim2]
                            - float(self.__num_bins) / 2.0 * self.__bin_widths[dim2],
                            self.__min_vals[dim2]
                            + float(self.__num_bins) / 2.0 * self.__bin_widths[dim2],
                            self.__bin_widths[dim2],
                        )

                    if self.__min_vals[dim1] != self.__max_vals[dim1]:
                        y_edges = np.arange(
                            self.__min_vals[dim1],
                            self.__min_vals[dim1]
                            + float(self.__num_bins) * self.__bin_widths[dim1],
                            self.__bin_widths[dim1],
                        )
                    else:
                        y_edges = np.arange(
                            self.__min_vals[dim1]
                            - float(self.__num_bins) / 2.0 * self.__bin_widths[dim1],
                            self.__min_vals[dim1]
                            + float(self.__num_bins) * self.__bin_widths[dim1],
                            self.__bin_widths[dim1],
                        )

                    H, x_edges, y_edges = np.histogram2d(x, y, bins=(x_edges, y_edges))
                    self.__den_val[index] = float(np.amax(H)) / float(len(x))
                    index = index + 1
        self.__writeCache()

    def consolidateCache(self, all_descriptors):
        """Update the min and max values with a new set"""
        min_vals_new_batch = all_descriptors.min(axis=0)
        max_vals_new_batch = all_descriptors.max(axis=0)

        self.__readCache()

        for ind in range(0, self.__ndim):
            if self.__max_vals[ind] < max_vals_new_batch[ind]:
                self.__max_vals[ind] = max_vals_new_batch[ind]
            if self.__min_vals[ind] > min_vals_new_batch[ind]:
                self.__min_vals[ind] = min_vals_new_batch[ind]

        # Bin width should not be changed
        index = 0
        for dim1 in range(0, self.__ndim):
            for dim2 in range(0, self.__ndim):
                if dim2 > dim1:
                    x = all_descriptors[:, dim2]
                    y = all_descriptors[:, dim1]
                    x_range = self.__max_vals[dim2] - self.__min_vals[dim2]
                    y_range = self.__max_vals[dim1] - self.__min_vals[dim1]
                    x_num_bins = np.ceil(float(x_range) / self.__bin_widths[dim2])
                    y_num_bins = np.ceil(float(y_range) / self.__bin_widths[dim1])
                    x_edges = np.arange(
                        self.__min_vals[dim2] - self.__bin_widths[dim2],
                        self.__min_vals[dim2]
                        + float(x_num_bins + 1) * self.__bin_widths[dim2],
                        self.__bin_widths[dim2],
                    )
                    y_edges = np.arange(
                        self.__min_vals[dim1] - self.__bin_widths[dim1],
                        self.__min_vals[dim1]
                        + float(y_num_bins + 1) * self.__bin_widths[dim1],
                        self.__bin_widths[dim1],
                    )
                    H, x_edges, y_edges = np.histogram2d(x, y, bins=(x_edges, y_edges))
                    max_den = float(np.amax(H)) / float(len(x))
                    if max_den > self.__den_val[index]:
                        self.__den_val[index] = max_den
                    index = index + 1

        self.__writeCache()

    def getMinValues(self):
        if len(self.__min_vals) == 0:
            self.__readCache()
        return self.__min_vals

    def getMaxValues(self):
        if len(self.__max_vals) == 0:
            self.__readCache()
        return self.__max_vals

    def getVmaxValues(self):
        if len(self.__den_val) == 0:
            self.__readCache()
        return self.__den_val

    def getBinWidths(self):
        if len(self.__bin_widths) == 0:
            self.__readCache()
        return self.__bin_widths


def main(**kwargs):
    path = kwargs.pop("path")
    file_name = kwargs.pop("file")
    path_and_file_name = path[0] + "/" + file_name[0]

    print("file name is {}".format(file_name))
    print("path is {}".format(path))
    print("path and file name is  {}".format(path_and_file_name))

    add = kwargs.pop("add")
    consolidate = kwargs.pop("consolidate")

    num_files = kwargs.pop("num_desc")
    if isinstance(num_files, list):
        num_files = num_files[0]

    num_bins = kwargs.pop("bins")
    if isinstance(num_bins, list):
        num_bins = num_bins[0]

    if add:
        cache = MinMaxDescCache(path_and_file_name, num_bins)
        print("number of files")
        print(num_files)
        desc_file_grp = panacea.descriptor_file_group.DescriptorFileGroup(
            kwargs.pop("desc_path")[0],
            kwargs.pop("desc_base")[0],
            kwargs.pop("desc_ext")[0],
        )
        desc_file_grp.read(0, num_files)
        all_descriptors = desc_file_grp.descriptors
        cache.createCache(all_descriptors)
    elif consolidate:
        # descriptor_files = read_all_descriptors.ReadAllDescriptors(num_files)
        desc_file_grp = panacea.descriptor_file_group.DescriptorFileGroup(
            kwargs.pop("desc_path")[0],
            kwargs.pop("desc_base")[0],
            kwargs.pop("desc_ext")[0],
        )
        desc_file_grp.read(0, num_files)
        all_descriptors = desc_file_grp.descriptors

        cache = MinMaxDescCache(path_and_file_name, num_bins)
        cache.consolidateCache(all_descriptors)

    read = kwargs.pop("read")
    if read:
        cache = MinMaxDescCache(path_and_file_name)
        print("Min Values")
        print(cache.getMinValues())
        print("Max Values")
        print(cache.getMaxValues())
        print("Max Den Values")
        print(cache.getVmaxValues())
        print("Bin Widths")
        print(cache.getBinWidths())


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        "min_max_desc_cache provides a means to"
        " cache the min and max descriptors. The purpose is to help with"
        " comparing plots, one can consolidate the min and max values between"
        " several datasets if needed. Furthermore, this script will also"
        " cache the maximum values of a plot of the different descriptors for"
        " the purpose of plotting isosurfaces and keeping the color of"
        " isosurface lines consistent between datasets."
    )

    desc = "Path to the cache directory. By default will use .panacea in the home directory"
    parser.add_argument(
        "--path", "-p", nargs=1, default=[os.getenv("HOME") + "/.panacea"], help=desc
    )

    desc = "Name of the cache file. By default will use minmax.cache."
    parser.add_argument("--file", "-f", nargs=1, default=["minmax.cache"], help=desc)

    desc = "Path to the descriptor files. By default will use the current working directory."
    parser.add_argument("--desc_path", "-dp", nargs=1, default=[os.getcwd()], help=desc)

    desc = (
        "Descriptor files base.\nDescriptor files are named with the following format\n"
        "base + index + extension\nFor example descriptors1.txt\n"
        "base = descriptors\nindex = 1\nextension = .txt\n"
        "By default the base is assumed to be descriptors"
    )
    parser.add_argument(
        "--desc_base", "-db", nargs=1, default=["descriptors"], help=desc
    )

    desc = (
        "Descriptor files extension.\nDescriptor files are named with the following format\n"
        "base + index + extension\nFor example descriptors1.txt\n"
        "base = descriptors\nindex = 1\nextension = .txt\n"
        "By default the extension is assumed to be .txt"
    )
    parser.add_argument("--desc_ext", "-de", nargs=1, default=[".txt"], help=desc)

    desc = "Consolidate the cache file with new data."

    parser.add_argument(
        "--consolidate",
        "-c",
        action="store_true",
        required=False,
        default=False,
        help=desc,
    )

    desc = "Add a new cache file."
    parser.add_argument(
        "--add", "-a", action="store_true", required=False, default=False, help=desc
    )

    desc = "Read the cache file."
    parser.add_argument(
        "--read", "-r", action="store_true", required=False, default=False, help=desc
    )

    desc = "Number of descriptor files to read from current directory. Default is 300"
    parser.add_argument(
        "--num_desc", "-n", type=int, required=False, default=300, help=desc
    )

    desc = "Number of bins to use in each direction. Default is 20"
    parser.add_argument("--bins", "-b", type=int, required=False, default=20, help=desc)

    args = parser.parse_args()

    try:
        main(**vars(args))
    except Exception:
        raise
