const $ = s => document.querySelector(s);
const u = p => encodeURI(p);
const TXT = {
  es: { projects: 'Proyectos', all: 'Todos', search: 'Buscar proyecto…', back: '← Volver', report: 'Reporte final', pres: 'Presentación', code: 'Ver código', photos: 'Fotos', video: 'Video', soon: 'Video próximamente', cv: 'Descargar CV', none: 'Sin resultados', hi: 'Hola, soy' },
  en: { projects: 'Projects', all: 'All', search: 'Search projects…', back: '← Back', report: 'Final report', pres: 'Presentation', code: 'View code', photos: 'Photos', video: 'Video', soon: 'Video coming soon', cv: 'Download CV', none: 'No results', hi: "Hi, I'm" }
};
let DATA, lang = localStorage.getItem('lang') || 'es', tag = null, q = '';
const t = k => TXT[lang][k];

async function init() {
  $('#y').textContent = new Date().getFullYear();
  try {
    DATA = await (await fetch('data/projects.json')).json();
  } catch (e) {
    $('#app').innerHTML = '<p style="padding:60px 0">No se pudo cargar data/projects.json. Abre el sitio desde GitHub Pages o con un servidor local (no con doble clic).</p>';
    return;
  }
  $('#lang').onclick = () => { lang = lang === 'es' ? 'en' : 'es'; localStorage.setItem('lang', lang); render(); };
  window.addEventListener('hashchange', () => { window.scrollTo(0, 0); render(); });
  render();
}

function render() {
  document.documentElement.lang = lang;
  $('#lang').textContent = lang === 'es' ? 'EN' : 'ES';
  const m = location.hash.match(/^#\/p\/(.+)$/);
  const p = m && DATA.projects.find(x => x.id === decodeURIComponent(m[1]));
  p ? detail(p) : home();
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
      <p>${P.role[lang]} · ${P.bio[lang]}</p>
      <a class="btn p" href="${u(P.cv)}" target="_blank">${t('cv')}</a>
      <a class="btn" href="${P.linkedin}" target="_blank">LinkedIn</a>
      <a class="btn" href="${P.github}" target="_blank">GitHub</a>
    </div>
  </section>
  <h2>${t('projects')}</h2>
  <div class="tools">
    <input id="q" placeholder="${t('search')}" value="${q}">
    <button class="chip ${tag ? '' : 'on'}" data-t="">${t('all')}</button>
    ${tags.map(g => `<button class="chip ${tag === g ? 'on' : ''}" data-t="${g}">#${g}</button>`).join('')}
  </div>
  <div class="grid" id="grid"></div>`;
  $('#q').oninput = e => { q = e.target.value; cards(); };
  document.querySelectorAll('.chip').forEach(c => c.onclick = () => { tag = c.dataset.t || null; home(); });
  cards();
}

function cards() {
  const ql = q.toLowerCase();
  const list = DATA.projects.filter(p =>
    (!tag || p.tags.includes(tag)) &&
    (!ql || (p.title[lang] + p.summary[lang] + p.tags.join(' ')).toLowerCase().includes(ql)));
  $('#grid').innerHTML = list.length ? list.map(p => `
    <a class="card" href="#/p/${encodeURIComponent(p.id)}">
      <div class="im"><img loading="lazy" src="${u(p.cover)}" alt=""></div>
      <div class="bd"><h3>${p.title[lang]}</h3><p>${p.summary[lang]}</p>
      ${p.tags.map(g => `<span class="tag">#${g}</span>`).join('')}</div>
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
    <p class="lead">${p.summary[lang]}</p>
    <div>${p.tags.map(g => `<span class="tag">#${g}</span>`).join('')}</div>
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
