import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="pyduino_bridge",
    version="1.0.0",
    author="Daniel Saromo Mori",
    author_email="danielsaromo@gmail.com",
    description="A transparent library for communication between Python and Arduino",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/DanielSaromo/PyDuino_Bridge",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: Apache License 2.0",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6',
)