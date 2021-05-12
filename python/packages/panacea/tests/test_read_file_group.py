
import panacea.read_descriptors
import panacea.read_files

import os
import pytest

def test_read_files():
    group_file_reader = panacea.read_files.ReadFiles(\
            os.path.dirname(os.path.abspath(__file__)),\
            "descriptors",\
            ".txt",\
            panacea.read_descriptors.DescriptorFile)

    group_file_reader.read()

    assert group_file_reader.base == "descriptors"
    assert group_file_reader.extension == ".txt"
    assert len(group_file_reader.available_files) == 3
    assert len(group_file_reader.files_read) == 3

    expected = ['descriptors1.txt', 'descriptors2.txt', 'descriptors3.txt']

    assert all([a == b for a, b in zip(group_file_reader.available_files, expected)])
    assert all([a == b for a, b in zip(group_file_reader.files_read, expected)])

    assert group_file_reader['descriptors1.txt'].descriptors[0,0] == 4
    assert group_file_reader['descriptors2.txt'].descriptors[0,0] == 5
    assert group_file_reader['descriptors3.txt'].descriptors[0,0] == 6

def test_read_files_selective():
    group_file_reader = panacea.read_files.ReadFiles(\
            os.path.dirname(os.path.abspath(__file__)),\
            "descriptors",\
            ".txt",\
            panacea.read_descriptors.DescriptorFile)

    # Only selectively read

    group_file_reader.read(2,4)
    assert group_file_reader.base == "descriptors"
    assert group_file_reader.extension == ".txt"
    assert len(group_file_reader.available_files) == 3
    assert len(group_file_reader.files_read) == 2

    expected = ['descriptors1.txt','descriptors2.txt', 'descriptors3.txt']
    expected_read = ['descriptors2.txt', 'descriptors3.txt']

    assert all([a == b for a, b in zip(group_file_reader.available_files, expected)])
    assert all([a == b for a, b in zip(group_file_reader.files_read, expected_read)])

    assert group_file_reader['descriptors2.txt'].descriptors[0,0] == 5
    assert group_file_reader['descriptors3.txt'].descriptors[0,0] == 6

    assert group_file_reader.indices_files_read[0] == 2
    assert group_file_reader.indices_files_read[1] == 3

