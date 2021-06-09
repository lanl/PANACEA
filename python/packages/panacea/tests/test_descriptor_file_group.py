import panacea.descriptor_file_group

import os
import pytest


def test_read_files():
    desc_file_reader = panacea.descriptor_file_group.DescriptorFileGroup(
        os.path.dirname(os.path.abspath(__file__)), "descriptors", ".txt"
    )

    desc_file_reader.read()

    min_vals, max_vals = desc_file_reader.minmax

    assert len(min_vals) == 3
    assert len(max_vals) == 3
    assert min_vals[0] == 1
    assert min_vals[1] == 2
    assert min_vals[2] == -4
    assert max_vals[0] == 6
    assert max_vals[1] == 2
    assert max_vals[2] == 3

    all_descriptors = desc_file_reader.descriptors

    rows, cols = all_descriptors.shape
    assert rows == 6
    assert cols == 3
    assert all_descriptors[0, 0] == 4
    assert all_descriptors[1, 0] == 1
    assert all_descriptors[2, 0] == 5
    assert all_descriptors[3, 0] == 1
    assert all_descriptors[4, 0] == 6
    assert all_descriptors[5, 0] == 1
    assert all_descriptors[0, 1] == 2
    assert all_descriptors[1, 1] == 2
    assert all_descriptors[2, 1] == 2
    assert all_descriptors[3, 1] == 2
    assert all_descriptors[4, 1] == 2
    assert all_descriptors[5, 1] == 2
    assert all_descriptors[0, 2] == 3
    assert all_descriptors[1, 2] == 3
    assert all_descriptors[2, 2] == 3
    assert all_descriptors[3, 2] == 3
    assert all_descriptors[4, 2] == 3
    assert all_descriptors[5, 2] == -4
