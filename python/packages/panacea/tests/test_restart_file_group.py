import panacea.restart_file_group

import os
import pytest

def test_read_files():
    restart_file_reader = panacea.restart_file_group.RestartFileGroup(\
            os.path.dirname(os.path.abspath(__file__)),\
            "cross_entropy_restart",\
            ".txt")

    restart_file_reader.read()

    expected = [
            'cross_entropy_restart1.txt',
            'cross_entropy_restart2.txt',
            'cross_entropy_restart3.txt']

    assert all([a == b for a, b in zip(restart_file_reader.files_read, expected)])


    assert len(restart_file_reader.files_read) == 3

    # Should grab self_entropy_restart1.txt
    assert restart_file_reader[1].entropy_type == "Cross"
    assert restart_file_reader[2].entropy_type == "Cross"
    assert restart_file_reader[3].entropy_type == "Cross"

    assert restart_file_reader[1].kernel_count == "Single"
    assert restart_file_reader[2].kernel_count == "Single"
    assert restart_file_reader[3].kernel_count == "Single"

    assert restart_file_reader[1].kernel_center_calculation == "Mean"
    assert restart_file_reader[2].kernel_center_calculation == "Mean"
    assert restart_file_reader[3].kernel_center_calculation == "Mean"

    assert len(restart_file_reader[1].kernel_centers) == 1
    assert len(restart_file_reader[2].kernel_centers) == 1
    assert len(restart_file_reader[3].kernel_centers) == 1

    assert restart_file_reader[1].kernel_centers[0,0] == 2.5
    assert restart_file_reader[2].kernel_centers[0,0] == 2.75
    assert restart_file_reader[3].kernel_centers[0,0] == 3.0
    assert restart_file_reader[1].kernel_centers[0,1] == 2
    assert restart_file_reader[2].kernel_centers[0,1] == 2
    assert restart_file_reader[3].kernel_centers[0,1] == 2
    assert restart_file_reader[1].kernel_centers[0,2] == 3
    assert restart_file_reader[2].kernel_centers[0,2] == 3
    assert restart_file_reader[3].kernel_centers[0,2] == 1.833

