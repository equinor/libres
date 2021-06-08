import setuptools
from setuptools_scm import get_version

version = get_version(
    relative_to=__file__,
    write_to="python/res/_version.py",
    write_to_template='# config: utf-8\n#\nversion = "{version}"',  # black-compatible version string
)

with open("README.md") as f:
    long_description = f.read()


setuptools.setup(
    name="equinor-libres",
    author="Equinor ASA",
    author_email="fg_sib-scout@equinor.com",
    description="Part of the Ensemble based Reservoir Tool (ERT)",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/equinor/libres",
    license="GPL-3.0",
    platforms="any",
    install_requires=[
        "cloudevents",
        "cwrap",
        "ecl",
        "jinja2",
        "numpy",
        "pandas",
        "psutil",
        "pyyaml",
        "requests",
        "websockets >= 9.0.1",
    ],
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Environment :: Other Environment",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: GNU General Public License v3 (GPLv3)",
        "Natural Language :: English",
        "Programming Language :: Python",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Topic :: Scientific/Engineering",
        "Topic :: Scientific/Engineering :: Physics",
        "Topic :: Software Development :: Libraries",
        "Topic :: Utilities",
    ],
    version=version,
    test_suite="tests",
)
