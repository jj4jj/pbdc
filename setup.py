from setuptools import setup,find_packages
setup(  
      name='pbdc',
      version='0.1.0',
      description="python describe data structure and represent with many tools like C/C++ syntax , proto syntax, json, ...etc",
      keywords='protobuf c++ c memory flat python',  
      author='jj4jj',
      author_email='resc@vip.qq.com',
      license='MIT License',
      py_modules=['pbdc'],
      packages = find_packages('./'),
      requires = ['Jinja2'],
      include_package_data = True,
      package_data = {'./':['templates/pb2.proto']},
      data_files = ['templates/pb2.proto'],
      url='https://github.com/jj4jj/pbdc',
)

