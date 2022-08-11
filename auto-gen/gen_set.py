fmt = """set* {key_type}_set_create(void) {{
    return dictionary_create(
        {key_type}_hash_function, {key_type}_compare,
        {key_type}_copy_constructor, {key_type}_destructor
    );
}}
"""

types = ["shallow", "string", "char", "double", "float", "int", "long", "short", "unsigned_char", "unsigned_int", "unsigned_long", "unsigned_short"]

def generate(key_type):
    print(fmt.format(key_type=key_type))
    
for k in types:
    generate(k)