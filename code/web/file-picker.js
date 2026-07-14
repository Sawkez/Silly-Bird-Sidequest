function openFilePicker() {
    document.getElementById('sq-file-input').click();
}

document.getElementById('sq-file-input').addEventListener('change', async (e) => {
    const file = e.target.files[0];
    if (!file) return;
    const buf = new Uint8Array(await file.arrayBuffer());
    FS.mkdirTree('/picked');
    FS.writeFile('/picked/' + file.name, buf);
    Module.ccall('on_file_picked', null, ['string'], ['/picked/' + file.name]);
    e.target.value = '';
});