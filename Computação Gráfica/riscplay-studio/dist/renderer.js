export class Screen {
  constructor(canvas) {
    this.canvas=canvas;this.gl=canvas.getContext('webgl',{alpha:false,antialias:false,preserveDrawingBuffer:true});
    if(!this.gl){this.ctx=canvas.getContext('2d');this.mode='Canvas 2D';return;}
    const gl=this.gl;this.mode='WebGL';
    const shader=(type,src)=>{const s=gl.createShader(type);gl.shaderSource(s,src);gl.compileShader(s);if(!gl.getShaderParameter(s,gl.COMPILE_STATUS))throw Error(gl.getShaderInfoLog(s));return s;};
    const p=gl.createProgram();gl.attachShader(p,shader(gl.VERTEX_SHADER,'attribute vec2 a; varying vec2 uv; void main(){gl_Position=vec4(a,0.,1.);uv=vec2((a.x+1.)*.5,(1.-a.y)*.5);}'));
    gl.attachShader(p,shader(gl.FRAGMENT_SHADER,'precision mediump float; varying vec2 uv; uniform sampler2D tex; void main(){gl_FragColor=texture2D(tex,uv);}'));
    gl.linkProgram(p);if(!gl.getProgramParameter(p,gl.LINK_STATUS))throw Error(gl.getProgramInfoLog(p));gl.useProgram(p);
    const buf=gl.createBuffer();gl.bindBuffer(gl.ARRAY_BUFFER,buf);gl.bufferData(gl.ARRAY_BUFFER,new Float32Array([-1,-1,1,-1,-1,1,1,1]),gl.STATIC_DRAW);
    const loc=gl.getAttribLocation(p,'a');gl.enableVertexAttribArray(loc);gl.vertexAttribPointer(loc,2,gl.FLOAT,false,0,0);
    const tex=gl.createTexture();gl.bindTexture(gl.TEXTURE_2D,tex);gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_MIN_FILTER,gl.NEAREST);gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_MAG_FILTER,gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_WRAP_S,gl.CLAMP_TO_EDGE);gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_WRAP_T,gl.CLAMP_TO_EDGE);gl.viewport(0,0,320,180);
    canvas.addEventListener('webglcontextlost',e=>{e.preventDefault();this.lost=true;});
    canvas.addEventListener('webglcontextrestored',()=>location.reload());
  }
  draw(pixels) {
    if(this.lost)return;
    if(this.gl){const gl=this.gl;gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,320,180,0,gl.RGBA,gl.UNSIGNED_BYTE,pixels);gl.drawArrays(gl.TRIANGLE_STRIP,0,4);}
    else this.ctx.putImageData(new ImageData(new Uint8ClampedArray(pixels),320,180),0,0);
  }
}
