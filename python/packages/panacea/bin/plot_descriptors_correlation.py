#!/usr/bin/python3

import argparse
import copy
import matplotlib.pyplot as plt
import matplotlib.tri as tri
import numpy as np
import os

import min_max_desc_cache
import panacea.descriptor_file_group
import panacea.restart_file_group

class DescriptorPlot:
    def __init__(self, **kwargs):
        self.__num_files = kwargs.pop('num_desc')
        if isinstance(self.__num_files,list):
            self.__num_files = self.__num_files[0]

        self.__plot_q_dist = not kwargs.pop('no_cross_ent')
        if isinstance(self.__plot_q_dist,list):
            self.__plot_q_dist = self.__plot_q_dist[0]

        self.__plot_p_dist = not kwargs.pop('no_self_ent')
        if isinstance(self.__plot_p_dist,list):
            self.__plot_p_dist = self.__plot_p_dist[0]

        self.__grid_res = kwargs.pop('grid_res')
        if isinstance(self.__grid_res,list):
            self.__grid_res = self.__grid_res[0]

        self.__dim_to_plot = kwargs.pop('plot_dims')
        if isinstance(self.__dim_to_plot,list):
            self.__dim_to_plot = self.__dim_to_plot[0]

        self.__display_plot = kwargs.pop('show')
        if isinstance(self.__display_plot,list):
            self.__display_plot = self.__display_plot[0]

        self.__levels = kwargs.pop('levels')
        if isinstance(self.__levels,list):
            self.__levels = self.__levels[0]

        self.__num_figs = kwargs.pop('figures')
        if isinstance(self.__num_figs,list):
            self.__num_figs = self.__num_figs[0]

        self.__plot_type = kwargs.pop('plot_type')
        if isinstance(self.__plot_type,list):
            self.__plot_type = self.__plot_type[0]

        self.__ticks_on = kwargs.pop('ticks_on')

        begin_end = self.__num_figs.split('-')

        self.__desc_file_base = kwargs.pop('desc_base')[0]
        self.__desc_file_ext = kwargs.pop('desc_extension')[0]
        self.__desc_file_path = kwargs.pop('desc_path')[0]
        self.__restart_file_base = kwargs.pop('restart_base')[0]
        self.__restart_file_ext = kwargs.pop('restart_extension')[0]
        self.__restart_file_path = kwargs.pop('restart_path')[0]

        self.__fig_start = int(begin_end[0])
        self.__fig_end = int(begin_end[1])

        self.__all_descriptor_files = panacea.descriptor_file_group.DescriptorFileGroup(
                self.__desc_file_path,
                self.__desc_file_base,
                self.__desc_file_ext)

        self.__all_descriptor_files.read()

        if self.__dim_to_plot > self.__all_descriptor_files.dimensions:
            self.__dim_to_plot = self.__all_descriptor_files.dimensions

        path = kwargs.pop('path')
        print("Path is %s " % path)

        self.__norm_iso_values_exist = False
        if path != 'N/A':
            # Load min max cache file
            file_name = kwargs.pop('file')
            path_and_file_name = path[0] + "/" + file_name[0]
            cache = min_max_desc_cache.MinMaxDescCache(path_and_file_name)
            self.__min_vals = cache.getMinValues()
            self.__max_vals = cache.getMaxValues()
            self.__bin_widths = cache.getBinWidths()
            print("Loading bin_widths")
            print(self.__bin_widths)
            self.__norm_iso_max_values_exist = True
            self.__norm_iso_max_values = cache.getVmaxValues()
        else:
            self.__min_vals, self.__max_vals = self.__all_descriptor_files.minmax

    def plot(self):

        print("Calling plot")
        all_restart_files = panacea.restart_file_group.RestartFileGroup(
                self.__restart_file_path,
                self.__restart_file_base,
                self.__restart_file_ext)

        all_restart_files.read()

        ndim = all_restart_files.dimensions

        plot_desc = True

        first_iteration = True
        for i in all_restart_files.indices_files_read:
            print("Restart file index i {}\n".format(i))
            print("Fig start {} and fig end {}".format(self.__fig_start,self.__fig_end))
            if i >= self.__fig_start and i <= self.__fig_end:
                print("Subplot dims to plot {} {}".format(self.__dim_to_plot-1,self.__dim_to_plot-1))
                # Squeeze false is used so that if only a single plot is possible of shape
                # 1 and 1 it won't collapse the axes
                fig, axs = plt.subplots(self.__dim_to_plot-1,self.__dim_to_plot-1, figsize=(10, 10), squeeze=False)
                print("Creting subplot")
                if self.__plot_q_dist and all_restart_files[i].entropy_type == "Cross":
                    print("Cross entropy plot")
                    covar_norm_q = copy.deepcopy(all_restart_files[i].covariance)
                    center_q = copy.deepcopy(all_restart_files[i].kernel_centers)[0]
                    print("Center q")
                    print(center_q)
                    norm_vals_q = copy.deepcopy(all_restart_files[i].normalization_coefficients)
                    print("Mean covar")
                    print(center_q)
                    # Normalize the covariance matrix and mean of the q distribution
                    for dim1 in range(0,ndim):
                        center_q[dim1] = center_q[dim1] * norm_vals_q[dim1]
                        for dim2 in range(0,ndim):
                            covar_norm_q[dim1][dim2] = covar_norm_q[dim1][dim2] * norm_vals_q[dim1]*norm_vals_q[dim2]

                    # Calculate covariance determinant
                    determin_q = np.linalg.det(covar_norm_q)
                    inv_covar_q = np.linalg.pinv(covar_norm_q)

                    pre_factor = 1.0 / np.sqrt(determin_q * np.power( 2.0 * np.pi,float(ndim) ))

                    for dim1 in range(0,self.__dim_to_plot-1):
                        for dim2 in range(1,self.__dim_to_plot):
                            if (dim2-1) < dim1:
                                axs[dim1][dim2-1].axis('off')
                            else:
                                vals_x = np.linspace(self.__min_vals[dim1]*0.9, self.__max_vals[dim1]*1.1,self.__grid_res, endpoint=True)
                                vals_y = np.linspace(self.__min_vals[dim2]*0.9, self.__max_vals[dim2]*1.1,self.__grid_res, endpoint=True)
                                vals_x_norm = copy.deepcopy(vals_x) * norm_vals_q[dim1]
                                vals_y_norm = copy.deepcopy(vals_y) * norm_vals_q[dim2]
                                X_vals, Y_vals = np.meshgrid(vals_y, vals_x)
                                data = np.zeros((self.__grid_res, self.__grid_res))
                                local_inv_covar = np.zeros((2,2))
                                local_inv_covar[0,0] = inv_covar_q[dim1,dim1]
                                local_inv_covar[1,0] = inv_covar_q[dim2,dim1]
                                local_inv_covar[0,1] = inv_covar_q[dim1,dim2]
                                local_inv_covar[1,1] = inv_covar_q[dim2,dim2]

                                center_q_new = np.zeros(2)
                                center_q_new[0] = center_q[dim1]
                                center_q_new[1] = center_q[dim2]
                                for j in range(0, self.__grid_res):
                                    for k in range(0, self.__grid_res):
                                        pt = np.array([ vals_x_norm[j], vals_y_norm[k] ])
                                        diff = pt-center_q_new
                                        data[k,j] = pre_factor * np.exp(-0.5*np.matmul(np.matmul(diff, local_inv_covar),diff.transpose()) )
                                print("X_vals")
                                print(X_vals)
                                print("Y_vals")
                                print(Y_vals)
                                print("data")
                                print(data.transpose())
                                print("X_vals shape")
                                print(X_vals.shape)
                                print("Y_vals shape")
                                print(Y_vals.shape)
                                print("data shape")
                                print(data.transpose().shape)
                                print("dim1 {} print dim2-1 {}\n".format(dim1,dim2-1))
                                axs[dim1,dim2-1].contour(X_vals, Y_vals, data.transpose(), self.__levels)

                if self.__plot_p_dist and all_restart_files[i].entropy_type == "Self":
                    covar_norm_p = copy.deepcopy(all_restart_files[i].covariance)
                    norm_vals_p = copy.deepcopy(all_restart_files[i].normalization_coefficients)

                    # Normalize the covariance matrix
                    for dim1 in range(0,ndim):
                        for dim2 in range(0,ndim):
                            covar_norm_p[dim1][dim2] = covar_norm_p[dim1][dim2] * norm_vals_p[dim1]*norm_vals_p[dim2]

                    # Calculate covariance determinant
                    determin_p = np.linalg.det(covar_norm_p)
                    inv_covar_p = np.linalg.inv(covar_norm_p)

                    kernels = self.__all_descriptor_files[i].number_descriptors

                    pre_factor = 1.0 / np.sqrt(determin_p * np.power( 2.0 * np.pi,float(ndim) ))

                    for dim1 in range(0,self.__dim_to_plot-1):

                        descriptors1 = self.__all_descriptor_files[i].descriptors[:,dim1]
                        for dim2 in range(1,self.__dim_to_plot):
                            descriptors2 = self.__all_descriptor_files[i].descriptors[:,dim2]
                            if (dim2-1) < dim1:
                                axs[dim1][dim2-1].axis('off')
                            else:
                                vals_x = np.linspace(self.__min_vals[dim1]*0.9, self.__max_vals[dim1]*1.1,self.__grid_res, endpoint=True)
                                vals_y = np.linspace(self.__min_vals[dim2]*0.9, self.__max_vals[dim2]*1.1,self.__grid_res, endpoint=True)
                                vals_x_norm = copy.deepcopy(vals_x) * norm_vals_p[dim1]
                                vals_y_norm = copy.deepcopy(vals_y) * norm_vals_p[dim2]
                                X_vals, Y_vals = np.meshgrid(vals_y, vals_x)
                                data = np.zeros((self.__grid_res, self.__grid_res))

                                local_inv_covar = np.zeros((2,2))
                                local_inv_covar[0,0] = inv_covar_p[dim1,dim1]
                                local_inv_covar[1,0] = inv_covar_p[dim2,dim1]
                                local_inv_covar[0,1] = inv_covar_p[dim1,dim2]
                                local_inv_covar[1,1] = inv_covar_p[dim2,dim2]
                                for j in range(0, self.__grid_res):
                                    for k in range(0, self.__grid_res):
                                        for kern in range(0,kernels):
                                            kern_mean = np.array([descriptors1[kern] * norm_vals_p[dim1],descriptors2[kern] * norm_vals_p[dim2]])
                                            pt = np.array([ vals_x_norm[j], vals_y_norm[k] ])
                                            diff = pt - kern_mean
                                            data[k,j] = data[k,j] + pre_factor * np.exp(-0.5*np.matmul(np.matmul(diff, local_inv_covar),diff.transpose()) )
                                axs[dim1][dim2-1].contour(X_vals, Y_vals, data.transpose(), self.__levels)

                descriptors_old = np.empty((1,ndim))

                if plot_desc:
                    iso_ind = 0
                    for dim1 in range(0,self.__dim_to_plot-1):
                        for dim2 in range(1,self.__dim_to_plot):
                            if (dim2-1) < dim1:
                                axs[dim1][dim2-1].axis('off')
                            else:
                                if first_iteration:
                                    descriptors_old = self.__all_descriptor_files[i].descriptors
                                else:
                                    descriptors_old = np.concatenate((descriptors_old, self.__all_descriptor_files[i].descriptors))

                                if self.__plot_type == "scatter":
                                    print("Plotting scatter")
                                    axs[dim1][dim2-1].scatter(descriptors_old[:,dim2],descriptors_old[:,dim1], s=0.5, color='blue',alpha=0.1)
                                else:
                                    descriptors_new_and_old = np.concatenate((descriptors_old, self.__all_descriptor_files[i+1].descriptors))
                                    x = descriptors_new_and_old[:,dim2]
                                    y = descriptors_new_and_old[:,dim1]
                                    x_range = self.__max_vals[dim2] - self.__min_vals[dim2]
                                    y_range = self.__max_vals[dim1] - self.__min_vals[dim1]
                                    x_num_bins = np.ceil(float(x_range) / self.__bin_widths[dim2])
                                    y_num_bins = np.ceil(float(y_range) / self.__bin_widths[dim1])
                                    x_edges = np.arange(self.__min_vals[dim2] - self.__bin_widths[dim2],self.__min_vals[dim2] + float(x_num_bins)*self.__bin_widths[dim2], self.__bin_widths[dim2])
                                    y_edges = np.arange(self.__min_vals[dim1] - self.__bin_widths[dim1],self.__min_vals[dim1] + float(y_num_bins)*self.__bin_widths[dim1], self.__bin_widths[dim1])
                                    H, xedges, yedges = np.histogram2d(x, y, bins=(x_edges,y_edges))

                                    H = H.T/float(len(x))
                                    X, Y = np.meshgrid(xedges, yedges)
                                    X = X[1:,1:] + self.__bin_widths[dim2]/2.0
                                    Y = Y[1:,1:] + self.__bin_widths[dim1]/2.0
                                    if self.__norm_iso_max_values_exist:
                                        levels = np.linspace(0.0,self.__norm_iso_max_values[iso_ind], num=self.__levels, endpoint=True)
                                        axs[dim1][dim2-1].contour(X, Y, H, levels=levels, vmin = 0.0, vmax = self.__norm_iso_max_values[iso_ind], cmap=plt.get_cmap('nipy_spectral'))
                                    else:
                                        axs[dim1][dim2-1].contour(X, Y, H, levels=self.__levels)

                                axs[dim1][dim2-1].set_xlim([self.__min_vals[dim2]*0.9, self.__max_vals[dim2]*1.1])
                                axs[dim1][dim2-1].set_ylim([self.__min_vals[dim1]*0.9, self.__max_vals[dim1]*1.1])
                                axs[dim1][dim2-1].axes.xaxis.set_visible(self.__ticks_on)
                                axs[dim1][dim2-1].axes.yaxis.set_visible(self.__ticks_on)
                    iso_ind = iso_ind + 1

                file_name = "descriptor_correlation"
                if self.__plot_p_dist:
                    file_name = file_name + "_p_"
                if self.__plot_q_dist:
                    file_name = file_name + "_q_"
                file_name = file_name + str(i) + ".png"
                plt.savefig(file_name)
                if self.__display_plot:
                    plt.show()
                plt.close()

            first_iteration = False

def main(**kwargs):
    correlated_plot = DescriptorPlot(**kwargs)
    correlated_plot.plot()

if __name__ == "__main__":
    parser = argparse.ArgumentParser("plot descriptors in correlation matrix plots.")

    desc = ('Path to the min max cache directory. By default the folder '\
    '.panacea in the HOME directory will be used.')
    parser.add_argument('--path','-p',
                        type=str,
                        nargs=1,
                        default=[os.getenv('HOME') + "/.panacea"],
                        help=desc)

    desc = ('Path to the descriptor file directory. By default will use the '\
            'current working directory.')
    parser.add_argument('--desc_path','-dp',
                        nargs=1,
                        default=[os.getcwd()],
                        help=desc)

    desc = ('Path to the restart file directory. By default will use the '\
            'current working directory.')
    parser.add_argument('--restart_path','-rp',
                        nargs=1,
                        default=[os.getcwd()],
                        help=desc)

    desc = ('Descriptor files base name.\nE.g. the files should have a base + index +'\
            ' extension format. For instance descriptors5.txt can be broken down into '\
            'base = descriptors, index = 5, and extension = .txt\n'\
            'By default the base is assumed to be descritpors')
    parser.add_argument('--desc_base','-db',
                        nargs=1,
                        default=["descriptors"],
                        help=desc)

    desc = ('Descriptor files extension.\nE.g. the files should have a base + index +'\
            ' extension format. For instance descriptors5.txt can be broken down into '\
            'base = descriptors, index = 5, and extension = .txt\n'\
            'By default the extension is assumed to be .ext')
    parser.add_argument('--desc_extension','-de',
                        nargs=1,
                        default=[".txt"],
                        help=desc)

    desc = ('Restart files base name.\nE.g. the files should have a base + index +'\
            ' extension format. For instance restart5.txt can be broken down into '\
            'base = restart, index = 5, and extension = .txt\n'\
            'By default the base is assumed to be restart')
    parser.add_argument('--restart_base','-rb',
                        nargs=1,
                        default=["restart"],
                        help=desc)

    desc = ('Restart files extension.\nE.g. the files should have a base + index +'\
            ' extension format. For instance restart5.txt can be broken down into '\
            'base = restart, index = 5, and extension = .txt\n'\
            'By default the extension is assumed to be .ext')
    parser.add_argument('--restart_extension','-re',
                        nargs=1,
                        default=[".txt"],
                        help=desc)

    desc = ('Name of the min max cache file. By default minmax.cache is used.')
    parser.add_argument('--file','-f',
                        type=str,
                        nargs=1,
                        default=["minmax.cache"],
                        help=desc)

    desc = ('Turn self entropy off.')
    parser.add_argument('--no_self_ent','-nse',
                        action="store_true",
                        required=False,
                        default=False,
                        help=desc)

    desc = ('Turn Cross entropy off.')
    parser.add_argument('--no_cross_ent','-nce',
                        action="store_true",
                        required=False,
                        default=False,
                        help=desc)

    desc = ('Number of levels in contour plot. Default is 10')
    parser.add_argument('--levels','-l',
                        type=int,
                        nargs=1,
                        required=False,
                        default=10,
                        help=desc)

    desc = ('How to plot the actual data, the default is a scatter plot,'\
            ' the other option is a contour plot. The z value of the isosurface'\
            ' is normalized by the number of points in each plot.')
    parser.add_argument('--plot_type','-pt',
            required=False,
            default="scatter",
            help=desc)

    desc = ('Number of descriptor files to read from current directory. Default is 300')
    parser.add_argument('--num_desc','-n',
                        type=int,
                        nargs=1,
                        required=False,
                        default=300,
                        help=desc)

    desc = ('Turn tick marks on.')
    parser.add_argument('--ticks_on','-ton',
                        action='store_true',
                        help=desc)

    desc = ('Steps to plot figures for. Default is "0-300", can be specified with range "4-8"')
    parser.add_argument('--figures','-fg',
            type=str,
            nargs=1,
            required=False,
            default="0-300",
            help=desc)

    desc = ('Number of descriptor dimensions to plot. Default is 5')
    parser.add_argument('--plot_dims','-d',
            type=int,
            nargs=1,
            required=False,
            default=5,
            help=desc)

    desc = ('Grid resolution of plot. Deafault is 41.')
    parser.add_argument('--grid_res','-g',
            type=int,
            nargs=1,
            required=False,
            default=41,
            help=desc)

    desc = ('Display plots.')
    parser.add_argument('--show','-s',
            action="store_true",
            required=False,
            default=False,
            help=desc)

    args = parser.parse_args()

    try:
        main(**vars(args))
    except Exception:
        raise
