import pytest

import panacea.read_restart


@pytest.fixture
def read_file():
    restart_file_io = panacea.read_restart.RestartFile()
    restart_file_io.read("tests/self_entropy_restart.txt")
    return restart_file_io


def test_entropy_type(read_file):
    assert read_file.entropy_type == "Self"


def test_distribution_type(read_file):
    assert read_file.distribution_type == "Kernel"


def test_prefactor(read_file):
    assert read_file.prefactor == 0.5


def test_correlated(read_file):
    assert read_file.correlated == False


def test_kernel_center_calculation(read_file):
    assert read_file.kernel_center_calculation == "None"


def test_primitive_type(read_file):
    assert read_file.primitive_type == "Gaussian"


def test_normalization_method(read_file):
    assert read_file.normalization_method == "None"


def test_normalization_coefficients(read_file):
    assert read_file.normalization_coefficients.size == 3
    assert read_file.normalization_coefficients[0] == 1.0
    assert read_file.normalization_coefficients[1] == 1.0
    assert read_file.normalization_coefficients[2] == 1.0


def test_kernel_centers(read_file):
    assert read_file.kernel_centers.shape[0] == 2
    assert read_file.kernel_centers.shape[1] == 3
    assert read_file.kernel_centers[0, 0] == 1.0
    assert read_file.kernel_centers[1, 0] == 1.0
    assert read_file.kernel_centers[0, 1] == 2.0
    assert read_file.kernel_centers[1, 1] == 2.0
    assert read_file.kernel_centers[0, 2] == 3.0
    assert read_file.kernel_centers[1, 2] == 3.0


def test_kernel_normalization_state(read_file):
    assert read_file.covariance_norm_state == "Normalized"


def test_covariance_total_points(read_file):
    assert read_file.covariance_total_pts == 2


def test_covariance(read_file):
    assert read_file.covariance.shape[0] == 3
    assert read_file.covariance.shape[1] == 3
    assert read_file.covariance[0, 0] == 1.0
    assert read_file.covariance[1, 0] == 0.0
    assert read_file.covariance[2, 0] == 0.0
    assert read_file.covariance[0, 1] == 0.0
    assert read_file.covariance[1, 1] == 1.0
    assert read_file.covariance[2, 1] == 0.0
    assert read_file.covariance[0, 2] == 0.0
    assert read_file.covariance[1, 2] == 0.0
    assert read_file.covariance[2, 2] == 1.0


def test_covariance_mean(read_file):
    assert read_file.covariance_mean.shape[0] == 3
    assert read_file.covariance_mean[0] == 1
    assert read_file.covariance_mean[1] == 2
    assert read_file.covariance_mean[2] == 3
