import pytest

import panacea.read_descriptors


@pytest.fixture
def read_file():
    descriptors_file_io = panacea.read_descriptors.DescriptorFile()
    descriptors_file_io.read("tests/descriptors.txt")
    return descriptors_file_io


def test_descriptors(read_file):
    assert read_file.descriptors.shape[0] == 2
    assert read_file.descriptors.shape[1] == 3
    assert read_file.descriptors[0, 0] == 1.0
    assert read_file.descriptors[1, 0] == 1.0
    assert read_file.descriptors[0, 1] == 2.0
    assert read_file.descriptors[1, 1] == 2.0
    assert read_file.descriptors[0, 2] == 3.0
    assert read_file.descriptors[1, 2] == 3.0
