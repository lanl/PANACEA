from setuptools import setup, find_packages

with open("README.md", "r") as f:
    long_description = f.read()

setup(
    name="panacea",
    description="PANACEA python scripts",
    long_description=long_description,
    author="Joshua S. Brown",
    author_email="joshbro42867@yahoo.com",
    license_files="LICENSE.txt",
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Users",
        "Environment :: Console",
        "Natural Language :: English",
        "Operating System :: Unix",
        "Programming Language :: Python :: 3.9",
    ],
    keywords="panacea, entropy, plotting",
    packages=find_packages(),
    install_requires=["matplotlib", "numpy"],
    scripts=[
        "bin/plot_entropy.py",
        "bin/plot_descriptors_correlation.py",
        "bin/plot_descriptors.py",
        "bin/min_max_desc_cache.py",
    ],
)
