// Browser QA against local files via Playwright's in-memory HTTP routing.
// No listening socket or remote service is used by this test harness.
const {chromium}=require(process.env.PLAYWRIGHT_MODULE || 'playwright');
const fs=require('node:fs'),path=require('node:path'),{spawn}=require('node:child_process');
const root=path.resolve(__dirname,'..');
const mime={'.js':'text/javascript','.json':'application/json','.html':'text/html','.css':'text/css','.png':'image/png'};
function backend(data){return new Promise((resolve,reject)=>{const p=spawn('python',[path.join(__dirname,'compile_stdin.py')],{stdio:['pipe','pipe','pipe']});let out='',err='';p.stdout.on('data',v=>out+=v);p.stderr.on('data',v=>err+=v);p.on('error',reject);p.on('exit',()=>{try{resolve(JSON.parse(out));}catch{reject(Error(err||out));}});p.stdin.end(JSON.stringify(data));});}
(async()=>{
 const browser=await chromium.launch({headless:true,executablePath:process.env.BROWSER_PATH || undefined});
 const context=await browser.newContext({viewport:{width:1440,height:1000},acceptDownloads:true});
 const errors=[];let builds=0,assets=0;
 await context.route('**/*',async route=>{
   const u=new URL(route.request().url());if(u.hostname!=='riscplay.test')return route.abort();
   if(u.pathname==='/api/status')return route.fulfill({json:{ok:true,compiler:true}});
   if(u.pathname==='/api/build'||u.pathname==='/api/assets'){
     if(u.pathname.endsWith('build'))builds++;else assets++;
     const result=await backend(route.request().postDataJSON());if(!result.ok)console.log('API ERROR',result.error);return route.fulfill({status:result.ok?200:422,json:result});
   }
   const file=path.resolve(root,'dist','.'+(u.pathname==='/'?'/index.html':u.pathname));
   if(!file.startsWith(path.join(root,'dist'))||!fs.existsSync(file))return route.fulfill({status:404,body:'Missing '+u.pathname});
   return route.fulfill({body:fs.readFileSync(file),contentType:mime[path.extname(file)]||'application/octet-stream'});
 });
 const page=await context.newPage();page.on('pageerror',e=>errors.push(e.message));
 await page.goto('http://riscplay.test/#key=test');await page.locator('#diagnostics').filter({hasText:'Exemplo pronto'}).waitFor({timeout:30000});
 await page.waitForFunction(()=>!document.querySelector('#frame-stat').textContent.startsWith('0 '),{timeout:30000});
 await page.screenshot({path:path.join(root,'reports/desktop.png'),fullPage:true});
 console.log('INITIAL',await page.locator('#diagnostics').innerText(),await page.locator('#frame-stat').innerText());
 await page.selectOption('#project','starter');await page.locator('#diagnostics').filter({hasText:'Exemplo pronto'}).waitFor();
 const source=JSON.parse(fs.readFileSync(path.join(root,'dist/examples/starter.json'))).files['main.c'];
 await page.locator('.cm-content').click();await page.keyboard.press('Control+A');await page.keyboard.insertText(source.replace('#define SPEED 2','#define SPEED 4'));
 await page.locator('#diagnostics').filter({hasText:'Compilação concluída'}).waitFor({timeout:30000}).catch(async e=>{console.log('DIAG',await page.locator('#diagnostics').innerText(),errors);throw e;});
 console.log('LIVE',builds,await page.locator('#diagnostics').innerText());
 const good=source.replace('#define SPEED 2','#define SPEED 4');await page.locator('.cm-content').click();await page.keyboard.press('Control+A');await page.keyboard.insertText(good+'\ninvalid syntax here!');
 await page.locator('#diagnostics').filter({hasText:'Corrija o código'}).waitFor({timeout:30000});
 if(!await page.locator('#export-prog').isDisabled())throw Error('Export must be disabled for stale build');
 await page.locator('.cm-content').click();await page.keyboard.press('Control+A');await page.keyboard.insertText(good);await page.locator('#diagnostics').filter({hasText:'Compilação concluída'}).waitFor({timeout:30000});
 await page.locator('#assets-tab').click();await page.locator('#image-input').setInputFiles(path.join(root,'tests/tiny.png'));
 await page.waitForFunction(()=>document.querySelector('#asset-summary').textContent.startsWith('1 importadas'));
 await page.locator('#diagnostics').filter({hasText:'Compilação concluída'}).waitFor({timeout:30000});
 const headerDownload=page.waitForEvent('download');await page.locator('#header-download').click();await(await headerDownload).saveAs(path.join(root,'reports/exported_assets.h'));
 await page.screenshot({path:path.join(root,'reports/assets.png'),fullPage:true});
 await page.locator('#sound').click();if(!/Som ligado/.test(await page.locator('#sound').innerText()))throw Error('Audio did not activate');
 await page.locator('#screen').focus();await page.keyboard.down('ArrowRight');await page.waitForTimeout(300);await page.keyboard.up('ArrowRight');
 await page.locator('#pause').click();if(!/Continuar/.test(await page.locator('#pause').innerText()))throw Error('Pause failed');await page.locator('#pause').click();
 const progDownload=page.waitForEvent('download');await page.locator('#export-prog').click();await(await progDownload).saveAs(path.join(root,'reports/exported.json.prog'));
 const exported=JSON.parse(fs.readFileSync(path.join(root,'reports/exported.json.prog')));if(exported.some(e=>!e.comp_uuid||!e.addr_mem||typeof e.value!=='string'))throw Error('Loader schema mismatch');
 await page.locator('#project-input').setInputFiles(path.join(root,'reports/exported.json.prog'));await page.locator('#diagnostics').filter({hasText:'Pacote carregado'}).waitFor();
 await page.waitForFunction(()=>!document.querySelector('#frame-stat').textContent.startsWith('0 '));
 await page.setViewportSize({width:390,height:844});await page.screenshot({path:path.join(root,'reports/mobile.png'),fullPage:true});
 const overflow=await page.evaluate(()=>document.documentElement.scrollWidth>innerWidth);if(overflow)throw Error('Mobile horizontal overflow');
 console.log(JSON.stringify({ok:errors.length===0,builds,assets,errors,mobileOverflow:overflow,loaderExportReimport:true}));
 fs.writeFileSync(path.join(root,'reports/browser.json'),JSON.stringify({ok:errors.length===0,builds,assets,errors,mobileOverflow:overflow,loaderExportReimport:true},null,2));
 await browser.close();if(errors.length)process.exitCode=1;
})().catch(e=>{console.error(e);process.exit(1);});
