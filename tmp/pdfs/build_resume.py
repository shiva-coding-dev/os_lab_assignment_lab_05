from reportlab.lib import colors
from reportlab.lib.enums import TA_LEFT, TA_RIGHT
from reportlab.lib.pagesizes import letter
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import inch
from reportlab.platypus import BaseDocTemplate, Frame, PageTemplate, Paragraph, Spacer, Table, TableStyle, KeepTogether
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.pdfbase import pdfmetrics

OUT = 'output/pdf/ShivaGupta_Optum_TDP_Resume.pdf'
PAGE_W, PAGE_H = letter
LEFT = RIGHT = 0.48 * inch
TOP = 0.38 * inch
BOTTOM = 0.35 * inch
WIDTH = PAGE_W - LEFT - RIGHT

styles = getSampleStyleSheet()
styles.add(ParagraphStyle(name='Name', fontName='Helvetica-Bold', fontSize=24, leading=25, textColor=colors.HexColor('#111111'), spaceAfter=1))
styles.add(ParagraphStyle(name='Contact', fontName='Helvetica', fontSize=8.8, leading=10.8, alignment=TA_RIGHT, textColor=colors.HexColor('#333333')))
styles.add(ParagraphStyle(name='Section', fontName='Helvetica-Bold', fontSize=11.3, leading=12.4, textColor=colors.HexColor('#111111'), spaceBefore=6.5, spaceAfter=2.8, keepWithNext=True))
styles.add(ParagraphStyle(name='Body', fontName='Helvetica', fontSize=9.0, leading=10.9, textColor=colors.HexColor('#111111'), spaceAfter=1.2))
styles.add(ParagraphStyle(name='BodyTight', fontName='Helvetica', fontSize=8.75, leading=10.35, textColor=colors.HexColor('#111111'), spaceAfter=0.8))
styles.add(ParagraphStyle(name='ResumeBullet', fontName='Helvetica', fontSize=8.7, leading=10.35, leftIndent=10, firstLineIndent=-7.5, textColor=colors.HexColor('#111111'), spaceAfter=0.7))
styles.add(ParagraphStyle(name='Entry', fontName='Helvetica-Bold', fontSize=9.35, leading=10.9, textColor=colors.HexColor('#111111')))
styles.add(ParagraphStyle(name='EntryRight', fontName='Helvetica', fontSize=8.7, leading=10.7, alignment=TA_RIGHT, textColor=colors.HexColor('#333333')))
styles.add(ParagraphStyle(name='Sub', fontName='Helvetica', fontSize=8.85, leading=10.5, textColor=colors.HexColor('#222222')))

def P(text, style='Body'):
    return Paragraph(text, styles[style])

def section(title):
    t = Table([[Paragraph(title.upper(), styles['Section'])]], colWidths=[WIDTH])
    t.setStyle(TableStyle([('LINEBELOW', (0,0), (-1,-1), 0.45, colors.HexColor('#333333')), ('BOTTOMPADDING', (0,0), (-1,-1), 1.5), ('TOPPADDING', (0,0), (-1,-1), 0)]))
    return t

def entry(title, right, subtitle=None):
    rows = [[P(title, 'Entry'), P(right, 'EntryRight')]]
    if subtitle:
        rows.append([P(subtitle, 'Sub'), ''])
    t = Table(rows, colWidths=[WIDTH*0.68, WIDTH*0.32])
    t.setStyle(TableStyle([('VALIGN', (0,0), (-1,-1), 'TOP'), ('ALIGN', (1,0), (1,-1), 'RIGHT'), ('LEFTPADDING', (0,0), (-1,-1), 0), ('RIGHTPADDING', (0,0), (-1,-1), 0), ('TOPPADDING', (0,0), (-1,-1), 0), ('BOTTOMPADDING', (0,0), (-1,-1), 0)]))
    return t

def bullet(text):
    return P('&bull;&nbsp;&nbsp;' + text, 'ResumeBullet')

class ResumeDoc(BaseDocTemplate):
    def __init__(self, filename):
        BaseDocTemplate.__init__(self, filename, pagesize=letter, leftMargin=LEFT, rightMargin=RIGHT, topMargin=TOP, bottomMargin=BOTTOM)
        frame = Frame(LEFT, BOTTOM, WIDTH, PAGE_H-TOP-BOTTOM, id='normal', leftPadding=0, rightPadding=0, topPadding=0, bottomPadding=0)
        self.addPageTemplates([PageTemplate(id='resume', frames=[frame])])

story = []
header = Table([[P('Shiva Gupta', 'Name'), P('+91 83539 40979<br/>shiva_2401mc42@iitp.ac.in<br/>India | LinkedIn<br/>github.com/shiva-coding-dev | shiva-portfolio', 'Contact')]], colWidths=[WIDTH*0.55, WIDTH*0.45])
header.setStyle(TableStyle([('VALIGN', (0,0), (-1,-1), 'TOP'), ('ALIGN', (1,0), (1,0), 'RIGHT'), ('LEFTPADDING', (0,0), (-1,-1), 0), ('RIGHTPADDING', (0,0), (-1,-1), 0), ('TOPPADDING', (0,0), (-1,-1), 0), ('BOTTOMPADDING', (0,0), (-1,-1), 0)]))
story += [header, Spacer(1, 4)]

story += [section('Education'), entry('Indian Institute of Technology, Patna', '2024 - 2028 (Expected)', 'B.Tech in Mathematics and Computing | CGPA: 7.34/10.0 | Patna, India')]
story += [P('<b>Relevant Coursework:</b> Data Structures & Algorithms, Machine Learning, Data Science, Probability & Statistics, Discrete Mathematics, Optimization', 'BodyTight')]

story += [section('Experience'), entry('Param Foundation', 'Remote, India', 'Full Stack Developer Intern | May 2026 - June 2026')]
story += [bullet('Led development and deployment of a production-ready full-stack NGO management platform using React, Django, and PostgreSQL (Supabase).'), bullet('Built cloud-based infrastructure with JWT authentication, certificate automation, scalable admin workflows, and services including Vercel, Render, and Resend.')]

story += [section('Projects')]
story += [entry('Param Foundation Website', 'Live URL')]
story += [bullet('Developed and deployed a full-stack platform serving 500+ monthly active users across volunteer, event, certificate, media, and supporter workflows.'), bullet('Implemented React, TypeScript, Django, PostgreSQL, JWT authentication, REST APIs, automated certificate emails, and cloud media management; maintained 99.9% uptime on Vercel, Render, and Resend.')]
story += [entry('DBMS Query Simulator with Benchmarking', 'Live URL')]
story += [bullet('<b>Technologies:</b> C++, Flask, React.js, SQL-like query parsing, B-Tree, hashing, two-phase locking (2PL), REST APIs, benchmarking, data visualization.'), bullet('Designed a custom DBMS engine supporting query parsing, execution, indexing, and comparative performance benchmarking across B-Tree and hash-based strategies.'), bullet('Deployed a C++ backend with a web interface for real-time query interaction and performance analysis.')]
story += [entry('Tewtr', 'GitHub')]
story += [bullet('Built a multi-agent AI pipeline combining LLaMA 4 Maverick (VLM) and GPT OSS 120B (LLM) to convert PDFs into readable educational content with contextual reasoning.'), bullet('Designed sliding-window inference with 3-page context, reducing document processing from 30 minutes to 12 seconds ideally while avoiding context overflow and information loss.'), bullet('Engineered high-throughput inference with providers such as SambaNova (600+ tokens/sec), achieving up to 5x faster processing than frontier AI in document reasoning tasks.')]

story += [section('Technical Skills')]
skills = [
    [P('<b>Languages:</b> Python, C++, JavaScript', 'BodyTight'), P('<b>Databases:</b> SQL, PostgreSQL, Supabase', 'BodyTight')],
    [P('<b>Software:</b> React.js, Django, REST APIs, TypeScript, Flask', 'BodyTight'), P('<b>Cloud & Tools:</b> AWS, Docker, Git/GitHub, Vercel, Render', 'BodyTight')],
    [P('<b>CS & AI:</b> DSA, OOP, DBMS, B-Trees, hashing, Machine Learning, LLMs', 'BodyTight'), P('<b>Practices:</b> JWT authentication, benchmarking, data visualization', 'BodyTight')],
]
st = Table(skills, colWidths=[WIDTH*0.52, WIDTH*0.48])
st.setStyle(TableStyle([('VALIGN', (0,0), (-1,-1), 'TOP'), ('LEFTPADDING', (0,0), (-1,-1), 0), ('RIGHTPADDING', (0,0), (-1,-1), 6), ('TOPPADDING', (0,0), (-1,-1), 0), ('BOTTOMPADDING', (0,0), (-1,-1), 0)]))
story += [st]

story += [section('Accomplishments')]
story += [bullet('Selected for Google Student Ambassador Program (1.0 & 2.0) and completed Google Upskilling Launchpad Program (2025).'), bullet('Qualified Round 2 of Google Big Code Competition; Codeforces rating 1040+; solved 150+ DSA problems across LeetCode, Codeforces, and CodeChef.'), bullet('JEE Mains: 97.14 percentile | JEE Advanced: 96.8 percentile | Bronze Medal at Inter IIT Cult 7.0; 6th in Streetplay.')]

doc = ResumeDoc(OUT)
doc.build(story)
print(OUT)
