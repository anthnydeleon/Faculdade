import sys,json
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import server
req=json.load(sys.stdin)
try:
    result=server.convert_images(req) if 'images' in req else server.compile_files(req['files'],server.find_compiler())
except Exception as e:result={'ok':False,'error':str(e)}
print(json.dumps(result))
