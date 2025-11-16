import sys
from pathlib import Path


if __name__ == '__main__':
    # Read
    h_file_path = Path(sys.argv[1])
    h_file_path = h_file_path.absolute()
    base_path = Path(__file__).resolve().parent
    h_file_data = h_file_path.read_text()
    h_file_data = h_file_data.strip().split('\n')
    # Replace define tags
    define_tag = '_'.join(
        h_file_path.relative_to(base_path).parts
    ).replace('.', '_').upper()
    if h_file_data[0].startswith('#ifndef'):
        h_file_data[0] = f'#ifndef {define_tag}'
        h_file_data[1] = f'#define {define_tag}'
        h_file_data[-1] = f'#endif //{define_tag}'
    # Replace namespace
    namespace = '::'.join(
        h_file_path.relative_to(base_path).parts
    ).removesuffix('.h')
    for i, line in enumerate(h_file_data):
        if line.startswith('namespace'):
            h_file_data[i] = f'namespace {namespace} {{'
            break
    # Replace class name
    class_name = h_file_path.stem
    for i, line in list(enumerate(h_file_data))[::-1]:
        if line.startswith(f'    class '):
            h_file_data[i] = f'    class {class_name} {{'
            break
        elif line.startswith(f'    struct'):
            h_file_data[i] = f'    struct {class_name} {{'
            break
    class_path = namespace + '::' + class_name
    # Write
    h_file_path.write_text(
        '\n'.join(h_file_data)
    )
    print(f'define_tag={define_tag}')
    print(f'namespace={namespace}')
    print(f'class_path={class_path}')
    print(f'class_name={class_name}')