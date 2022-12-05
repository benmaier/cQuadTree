#This file is forked from https://github.com/pybind/pbtest, original author: Sylvain Corlay

from setuptools import setup, Extension
import setuptools
from setuptools.command.build_ext import build_ext
import os, sys

# get __version__, __author__, and __email__
exec(open("./cQuadTree/metadata.py").read())

class get_pybind_include(object):
    """Helper class to determine the pybind11 include path
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked. """

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)

ext_modules = [
    Extension(
        '_cQuadTree',
        [
            '_cQuadTree/_cQuadTree.cpp',
        ],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
            "./_cQuadTree/"
        ],
        language='c++',
    ),
]

def has_flag(compiler, flagname):
    """Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    import tempfile
    fd, fname = tempfile.mkstemp('.cpp', 'main', text=True)
    with os.fdopen(fd, 'w') as f:
        f.write('int main (int argc, char **argv) { return 0; }')
    try:
        compiler.compile([fname], extra_postargs=[flagname])
    except setuptools.distutils.errors.CompileError:
        return False
    return True

def cpp_flag(compiler):
    """Return the -std=c++[11/14] compiler flag.
    The c++14 is preferred over c++11 (when it is available).
    """
    if has_flag(compiler, '-std=c++14'):
        return '-std=c++14'
    elif has_flag(compiler, '-std=c++11'):
        return '-std=c++11'
    else:
        raise RuntimeError('Unsupported compiler -- at least C++11 support is needed!')


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }

    if sys.platform == 'darwin':
        c_opts['unix'] += ['-stdlib=libc++', '-mmacosx-version-min=10.7','-ftemplate-depth=1024']

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        if ct == 'unix':
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')
        for ext in self.extensions:
            ext.extra_compile_args = opts
        build_ext.build_extensions(self)

setup(
    name = 'cQuadTree',
    version = __version__,
    author = __author__,
    author_email = __email__,
    url = 'https://github.com/benmaier/cQuadTree',
    license = __license__,
    description = 'Python API to a C++-implementation of a BarnesHutTree/QuadTree',
    long_description = '',
    packages = setuptools.find_packages(),
    ext_modules = ext_modules,
    tests_require=['pytest', 'pytest-cov'],
    setup_requires = [
            'pybind11>=2.0.0'
            'pytest-runner'
            ],
    install_requires = [
            'pybind11>=2.0.0',
            'numpy',
            'scipy',
            ],
    classifiers=['License :: OSI Approved :: MIT License',
                 'Programming Language :: Python :: 3.6',
                 'Programming Language :: Python :: 3.7',
                 'Programming Language :: Python :: 3.8',
                 ],
    project_urls={
        'Documentation': 'https://github.com/benmaier/cQuadTree',
        'Contributing Statement': 'https://github.com/benmaier/cQuadTree/blob/master/CONTRIBUTING.md',
        'Bug Reports': 'https://github.com/benmaier/cQuadTree/issues',
        'Source': 'https://github.com/benmaier/cQuadTree/',
        'PyPI': 'https://pypi.org/project/cQuadTree/',
    },
    include_package_data = True,
    cmdclass = {'build_ext': BuildExt},
    zip_safe = False,
)
