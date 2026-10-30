const $ = s => document.querySelector(s);
const u = p => /^https?:\/\//.test(p) ? p : encodeURI(p);
const TXT = {
  de: { exp: 'Berufserfahrung', projects: 'Projekte', certs: 'Zertifikate, Auszeichnungen & Empfehlungsschreiben', navc: 'Zertifikate & Awards', all: 'Alle', search: 'Projekt suchen…', back: '← Zurück', report: 'Abschlussbericht', cad: 'CAD-Dateien', pres: 'Präsentation', code: 'Code ansehen', photos: 'Fotos', video: 'Video', videos: 'Videos', soon: 'Video folgt in Kürze', cv: 'Lebenslauf herunterladen', none: 'Keine Ergebnisse', hi: 'Hallo, ich bin', empty: 'Zertifikate und Empfehlungsschreiben folgen in Kürze.', open: 'Öffnen', cert: 'Zertifikat', award: 'Auszeichnung', letter: 'Empfehlungsschreiben', studio: 'Studio-5000-Projekt herunterladen', ladder: 'Ladder-Diagramm (PDF)', emulate: 'Emulate-3D-Datei herunterladen', solidworks: 'CAD-Dateien' },
  en: { exp: 'Work Experience', projects: 'Projects', certs: 'Certifications, Awards & Recommendation Letters', navc: 'Certifications & Awards', all: 'All', search: 'Search projects…', back: '← Back', report: 'Final report', cad: 'CAD files', pres: 'Presentation', code: 'View code', photos: 'Photos', video: 'Video', videos: 'Videos', soon: 'Video coming soon', cv: 'Download CV', none: 'No results', hi: "Hi, I'm", empty: 'Certificates and recommendation letters coming soon.', open: 'Open', cert: 'Certificate', award: 'Award', letter: 'Recommendation letter', studio: 'Download Studio 5000 project', ladder: 'Ladder diagram (PDF)', emulate: 'Download Emulate 3D file', solidworks: 'CAD files' },
  es: { exp: 'Experiencia laboral', projects: 'Proyectos', certs: 'Certificaciones, reconocimientos y cartas de recomendación', navc: 'Certificaciones y reconocimientos', all: 'Todos', search: 'Buscar proyecto…', back: '← Volver', report: 'Reporte final', cad: 'Archivos CAD', pres: 'Presentación', code: 'Ver código', photos: 'Fotos', video: 'Video', videos: 'Videos', soon: 'Video próximamente', cv: 'Descargar CV', none: 'Sin resultados', hi: 'Hola, soy', empty: 'Certificados y cartas de recomendación próximamente.', open: 'Abrir', cert: 'Certificado', award: 'Reconocimiento', letter: 'Carta de recomendación', studio: 'Descargar proyecto de Studio 5000', ladder: 'Ladder de Studio 5000 (PDF)', emulate: 'Descargar archivo de Emulate 3D', solidworks: 'Archivos CAD' }
};
let DATA, lang = localStorage.getItem('lang') || 'de', tag = null, q = '', pendingScroll = null;
const t = k => TXT[lang][k];
const tn = k => (DATA.tagNames[k] || {})[lang] || k;
// CV por idioma: DE -> versión en alemán; EN y ES -> versión en inglés
const cvFile = P => (typeof P.cv === 'string') ? P.cv : (P.cv[lang] || P.cv.en);
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
    pendingScroll = a.dataset.s;
    if (location.hash.length > 2) return; // hashchange will render
    setTimeout(() => { render(); }, 0);
  });
  window.addEventListener('hashchange', () => { if (!pendingScroll) window.scrollTo(0, 0); render(); });
  render();
}

function render() {
  document.documentElement.lang = lang;
  document.querySelectorAll('.langs button').forEach(b => b.classList.toggle('on', b.dataset.l === lang));
  $('#n0').textContent = t('exp'); $('#n1').textContent = t('projects'); $('#n2').textContent = t('navc');
  const m = location.hash.match(/^#\/p\/(.+)$/);
  const p = m && DATA.projects.find(x => x.id === decodeURIComponent(m[1]));
  p ? detail(p) : home();
  if (pendingScroll && !p) { const el = document.getElementById(pendingScroll); el && el.scrollIntoView({ behavior: 'smooth' }); }
  pendingScroll = null;
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
      <a class="btn p" href="${u(cvFile(P))}" target="_blank">${t('cv')}</a>
      <a class="btn" href="${P.linkedin}" target="_blank">LinkedIn</a>
      <a class="btn" href="${P.github}" target="_blank">GitHub</a>
    </div>
  </section>
  ${experience()}
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

function experience() {
  return (DATA.experience || []).map(x => `
  <h2 id="exp">${t('exp')}</h2>
  <section class="xp">
    <div class="xp-side">
      <img src="${u(x.photo)}" alt="${x.company}" onerror="this.parentNode.classList.add('nophoto');this.remove()">
      <div class="xp-brand">${x.company}</div>
      <span class="chip">${x.sector[lang]}</span>
    </div>
    <div class="xp-main">
      <h3>${x.role[lang]} <span>@ ${x.company}</span>${x.period ? `<small>${x.period[lang] || x.period}</small>` : ''}</h3>
      <p class="xp-intro">${x.intro[lang]}</p>
      ${x.items.map(i => `<div class="xp-item">
        <div class="xp-t"><h4>${i.title[lang]}</h4><span class="tag">${i.tech}</span></div>
        <p>${i.text[lang]}</p>
        ${i.stat ? `<div class="stat"><b>${i.stat.value}</b><span>${i.stat.label[lang]}</span></div>` : ''}
      </div>`).join('')}
      <p class="xp-outro">${x.outro[lang]}</p>
    </div>
  </section>`).join('');
}

function certs() {
  const C = DATA.certifications || [];
  if (!C.length) return `<div class="cert empty">${t('empty')}</div>`;
  return C.map(c => {
    const kind = c.type === 'letter' ? 'letter' : c.type === 'award' ? 'award' : 'cert';
    return `<div class="cert"><span class="k">${t(kind)}</span>
    <strong>${c.title[lang]}</strong><small>${c.issuer || ''}${c.date ? ' · ' + c.date : ''}</small>
    ${c.note ? `<small style="display:block;margin-top:6px">${c.note[lang]}</small>` : ''}
    ${c.file ? `<a class="btn" style="margin-top:8px;text-align:center" href="${u(c.file)}" target="_blank">${t('open')}</a>` : ''}</div>`;
  }).join('');
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
  const vids = p.videos && p.videos.length ? p.videos : (p.video ? [{ id: p.video }] : []);
  const vid = vids.length
    ? vids.map(v => `${v.title ? `<p style="margin:16px 0 8px;font-weight:600">${v.title[lang]}</p>` : ''}<iframe class="vid" src="https://www.youtube.com/embed/${v.id}" allowfullscreen loading="lazy"></iframe>`).join('')
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
      ${p.cad ? `<a class="btn" href="${p.cad}" target="_blank">${t('cad')}</a>` : ''}
      ${(p.downloads || []).map(d => `<a class="btn" href="${u(d.file)}" ${d.download ? 'download' : 'target="_blank"'}>${t(d.k)}</a>`).join('')}
    </div>
    <h2>${vids.length > 1 ? t('videos') : t('video')}</h2>${vid}
    <h2>${t('photos')}</h2>
    <div class="gal">${p.photos.map(f => `<a href="${u(f)}" target="_blank"><img loading="lazy" src="${u(f)}" alt=""></a>`).join('')}</div>
  </div>
  <div style="height:50px"></div>`;
}

init();
