const $ = s => document.querySelector(s);
const u = p => encodeURI(p);
const TXT = {
  de: { projects: 'Projekte', certs: 'Zertifikate & Empfehlungsschreiben', navc: 'Zertifikate', all: 'Alle', search: 'Projekt suchen…', back: '← Zurück', report: 'Abschlussbericht', pres: 'Präsentation', code: 'Code ansehen', photos: 'Fotos', video: 'Video', soon: 'Video folgt in Kürze', cv: 'Lebenslauf herunterladen', none: 'Keine Ergebnisse', hi: 'Hallo, ich bin', empty: 'Zertifikate und Empfehlungsschreiben folgen in Kürze.', open: 'Öffnen', cert: 'Zertifikat', letter: 'Empfehlungsschreiben' },
  en: { projects: 'Projects', certs: 'Certifications & Recommendation Letters', navc: 'Certifications', all: 'All', search: 'Search projects…', back: '← Back', report: 'Final report', pres: 'Presentation', code: 'View code', photos: 'Photos', video: 'Video', soon: 'Video coming soon', cv: 'Download CV', none: 'No results', hi: "Hi, I'm", empty: 'Certificates and recommendation letters coming soon.', open: 'Open', cert: 'Certificate', letter: 'Recommendation letter' },
  es: { projects: 'Proyectos', certs: 'Certificaciones y cartas de recomendación', navc: 'Certificaciones', all: 'Todos', search: 'Buscar proyecto…', back: '← Volver', report: 'Reporte final', pres: 'Presentación', code: 'Ver código', photos: 'Fotos', video: 'Video', soon: 'Video próximamente', cv: 'Descargar CV', none: 'Sin resultados', hi: 'Hola, soy', empty: 'Certificados y cartas de recomendación próximamente.', open: 'Abrir', cert: 'Certificado', letter: 'Carta de recomendación' }
};
let DATA, lang = localStorage.getItem('lang') || 'de', tag = null, q = '', scrollTo = null;
const t = k => TXT[lang][k];
const tn = k => (DATA.tagNames[k] || {})[lang] || k;
const tg = g => `<span class="tag">#${tn(g)}</span>`;

async function init() {
  $('#y').textContent = new Date().getFullYear();
  try {
    DATA = await (await fetch('projects.json', { cache: 'no-store' })).json();
  } catch (e) {
    $('#app').innerHTML = '<p style="padding:60px 0">projects.json konnte nicht geladen werden / No se pudo cargar projects.json.</p>';
    return;
  }
  document.querySelectorAll('.langs button').forEach(b => b.onclick = () => { lang = b.dataset.l; localStorage.setItem('lang', lang); render(); });
  document.querySelectorAll('nav a[data-s]').forEach(a => a.onclick = () => {
    scrollTo = a.dataset.s;
    if (location.hash.length > 2) return; // hashchange will render
    setTimeout(() => { render(); }, 0);
  });
  window.addEventListener('hashchange', () => { if (!scrollTo) window.scrollTo(0, 0); render(); });
  render();
}

function render() {
  document.documentElement.lang = lang;
  document.querySelectorAll('.langs button').forEach(b => b.classList.toggle('on', b.dataset.l === lang));
  $('#n1').textContent = t('projects'); $('#n2').textContent = t('navc');
  const m = location.hash.match(/^#\/p\/(.+)$/);
  const p = m && DATA.projects.find(x => x.id === decodeURIComponent(m[1]));
  p ? detail(p) : home();
  if (scrollTo && !p) { const el = document.getElementById(scrollTo); el && el.scrollIntoView({ behavior: 'smooth' }); }
  scrollTo = null;
}

function home() {
  const P = DATA.profile;
  const tags = [...new Set(DATA.projects.flatMap(p => p.tags))];
  $('#app').innerHTML = `
  <section class="hero">
    <img src="${u(P.photo)}" alt="${P.name}" onerror="this.style.visibility='hidden'">
    <div>
      <p style="margin:0;color:var(--ac)">${t('hi')}</p>
      <h1><em>${P.name}</em></h1>
      <p><strong>${P.role[lang]}</strong><br>${P.bio[lang]}</p>
      <a class="btn p" href="${u(P.cv)}" target="_blank">${t('cv')}</a>
      <a class="btn" href="${P.linkedin}" target="_blank">LinkedIn</a>
      <a class="btn" href="${P.github}" target="_blank">GitHub</a>
    </div>
  </section>
  <h2 id="projects">${t('projects')}</h2>
  <div class="tools">
    <input id="q" placeholder="${t('search')}" value="${q}">
    <button class="chip ${tag ? '' : 'on'}" data-t="">${t('all')}</button>
    ${tags.map(g => `<button class="chip ${tag === g ? 'on' : ''}" data-t="${g}">#${tn(g)}</button>`).join('')}
  </div>
  <div class="grid" id="grid"></div>
  <h2 id="certs">${t('certs')}</h2>
  <div class="cgrid">${certs()}</div>`;
  $('#q').oninput = e => { q = e.target.value; cards(); };
  document.querySelectorAll('.chip').forEach(c => c.onclick = () => { tag = c.dataset.t || null; home(); });
  cards();
}

function certs() {
  const C = DATA.certifications || [];
  if (!C.length) return `<div class="cert empty">${t('empty')}</div>`;
  return C.map(c => `<div class="cert"><span class="k">${t(c.type === 'letter' ? 'letter' : 'cert')}</span>
    <strong>${c.title[lang]}</strong><small>${c.issuer || ''}${c.date ? ' · ' + c.date : ''}</small>
    ${c.file ? `<a class="btn" style="margin-top:8px;text-align:center" href="${u(c.file)}" target="_blank">${t('open')}</a>` : ''}</div>`).join('');
}

function cards() {
  const ql = q.toLowerCase();
  const list = DATA.projects.filter(p =>
    (!tag || p.tags.includes(tag)) &&
    (!ql || (p.title[lang] + p.summary[lang] + p.tags.map(tn).join(' ')).toLowerCase().includes(ql)));
  $('#grid').innerHTML = list.length ? list.map(p => `
    <a class="card" href="#/p/${encodeURIComponent(p.id)}">
      <div class="im"><img loading="lazy" src="${u(p.cover)}" alt=""></div>
      <div class="bd"><h3>${p.title[lang]}</h3><p>${p.summary[lang]}</p>${p.tags.map(tg).join('')}</div>
    </a>`).join('') : `<p>${t('none')}</p>`;
}

function detail(p) {
  const vid = p.video
    ? `<iframe class="vid" src="https://www.youtube.com/embed/${p.video}" allowfullscreen loading="lazy"></iframe>`
    : `<div class="soon">${t('soon')}</div>`;
  $('#app').innerHTML = `
  <div class="det">
    <a class="back" href="#/">${t('back')}</a>
    <img class="cover" src="${u(p.cover)}" alt="">
    <h1>${p.title[lang]}</h1>
    <div>${p.tags.map(tg).join('')}</div>
    <div class="long" style="margin-top:14px">${(p.long || p.summary)[lang].split('\n\n').map(x => `<p>${x}</p>`).join('')}</div>
    <div style="margin:18px 0">
      ${p.report ? `<a class="btn p" href="${u(p.report)}" target="_blank">${t('report')}</a>` : ''}
      ${p.presentation ? `<a class="btn" href="${u(p.presentation)}" target="_blank">${t('pres')}</a>` : ''}
      ${p.code ? `<a class="btn" href="${p.code}" target="_blank">${t('code')}</a>` : ''}
    </div>
    <h2>${t('video')}</h2>${vid}
    <h2>${t('photos')}</h2>
    <div class="gal">${p.photos.map(f => `<a href="${u(f)}" target="_blank"><img loading="lazy" src="${u(f)}" alt=""></a>`).join('')}</div>
  </div>
  <div style="height:50px"></div>`;
}

init();
