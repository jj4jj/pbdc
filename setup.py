from setuptools import setup,find_packages
setup(  
      name='pbdc',
      version='0.1.2',
      description="python describe data structure and represent with many tools like C/C++ syntax , proto syntax, json, ...etc",
      keywords=['protobuf','c++','c','data represent'],  
      author='jj4jj',
      author_email='resc@vip.qq.com',
      license='MIT License',
      packages = ['pbdc'],
      requires = ['Jinja2'],
      include_package_data = True,
      url='https://github.com/jj4jj/pbdc',
)

